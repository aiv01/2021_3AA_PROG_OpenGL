#include "DeferredDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "wobj.h"


void DeferredDraw::start() {
    m_sceneProgram = GLProgram::createByShaderName("deferredgbuffer");

    wobj* mesh = wobj_from_file("resources/models/stormtrooper.obj");

    //4. Load Vertex data to GPU
    for(int i=0; i < mesh->triangles_count; i++) {
        wobj_triangle t = mesh->triangles[i];
        m_vertices.push_back(t.v1.position.x);
        m_vertices.push_back(t.v1.position.y);
        m_vertices.push_back(t.v1.position.z);
        m_vertices.push_back(t.v1.uv.x);
        m_vertices.push_back(t.v1.uv.y);
        m_vertices.push_back(t.v1.normal.x);
        m_vertices.push_back(t.v1.normal.y);
        m_vertices.push_back(t.v1.normal.z);

        m_vertices.push_back(t.v2.position.x);
        m_vertices.push_back(t.v2.position.y);
        m_vertices.push_back(t.v2.position.z);
        m_vertices.push_back(t.v2.uv.x);
        m_vertices.push_back(t.v2.uv.y);
        m_vertices.push_back(t.v2.normal.x);
        m_vertices.push_back(t.v2.normal.y);
        m_vertices.push_back(t.v2.normal.z);

        m_vertices.push_back(t.v3.position.x);
        m_vertices.push_back(t.v3.position.y);
        m_vertices.push_back(t.v3.position.z);
        m_vertices.push_back(t.v3.uv.x);
        m_vertices.push_back(t.v3.uv.y);
        m_vertices.push_back(t.v3.normal.x);
        m_vertices.push_back(t.v3.normal.y);
        m_vertices.push_back(t.v3.normal.z);
    }


    //- Create Vertex Array (VAO)
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //- Create Buffer to load vertex data (VBO)
    glGenBuffers(1, &m_vbo_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertex);
    int dataSize = m_vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, m_vertices.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //- VAO link VBO to Vertex Shader
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(location_1);

    //- VAO link VBO to Vertex Shader
    GLuint location_2 = 2;
    glVertexAttribPointer(location_2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(location_2);

    //- Texture
    m_boxText = new GLTexture("resources/models/stormtrooper.png");

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    //glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    /*
    program->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    m_boxText->bind(1);
    */
    m_elapsedTime = 0.f;

    //camera
    glm::vec3 position = glm::vec3(0.f, 0.f, 5.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    float fovY = 60.f;
    float aspectRatio = 640.f / 480.f;
    float zNear = 0.1f;
    float zFar = 100.f;

    m_view = glm::lookAt(position, position + direction, up);
    m_projection = glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar);


    glClearColor(0.f, 0.f, 0.f, 1.f);
    int width = 640;
    int height = 480;

    //G-Buffer (alias Geometry-Buffer)
    //1. Rendering Scena solito, solo verso il framebuffer intermedio
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    // Create texture as output for POSITION (ATTACH 0)
    glGenTextures(1, &m_positionText);
    glBindTexture(GL_TEXTURE_2D, m_positionText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionText, 0);

    // Create texture as output for NORMALS (ATTACH 1)
    glGenTextures(1, &m_normalText);
    glBindTexture(GL_TEXTURE_2D, m_normalText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalText, 0);

    // Create texture as output for DIFFUSE (ATTACH 2)
    glGenTextures(1, &m_diffuseText);
    glBindTexture(GL_TEXTURE_2D, m_diffuseText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_diffuseText, 0);
    
    // MRT 
    GLenum attach[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attach);

    // Create render buffer as output for DEPTH attachement
    glGenRenderbuffers(1, &m_gBufferRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_gBufferRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gBufferRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) DIE("Framebuffer not completed!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Pipeline Rendering
    m_quadProgram = GLProgram::createByShaderName("deferredquad");
    std::vector<float> quad = {
       -1.0f, -1.0f, 0.0f, 0.0f,  //bottom-left
        1.0f, -1.0f, 1.0f, 0.0f,  //bottom-right
       -1.0f,  1.0f, 0.0f, 1.0f,  //top-left

       -1.0f,  1.0f, 0.0f, 1.0f,  //top-left
        1.0f, -1.0f, 1.0f, 0.0f,  //bottom-right
        1.0f,  1.0f, 1.0f, 1.0f,  //top-right
    };

    //- Create Vertex Array (VAO)
    glGenVertexArrays(1, &m_quadVao);
    glBindVertexArray(m_quadVao);

    //- Create Buffer to load vertex data (VBO)
    glGenBuffers(1, &m_quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
    glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float), quad.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2 *sizeof(float)));
    glEnableVertexAttribArray(1);


    //glm::vec3 pointLightPos = glm::vec3(10.f, 0.f, 0.f);
    m_quadProgram->bind();
    m_quadProgram->setUniform("pointLightPos[0]", glm::vec3(4.f, 0.f, 0.f));
    m_quadProgram->setUniform("pointLightPos[1]", glm::vec3(-4.f, 0.f, 0.f));

    m_quadProgram->setUniform("cameraPos", position);
    m_quadProgram->setUniform("gPosition", 0);
    m_quadProgram->setUniform("gNormal", 1);
    m_quadProgram->setUniform("gDiffuse", 2);
}

void DeferredDraw::update(float deltaTime) {
    //1. Render scene to G-Buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    m_elapsedTime += deltaTime;
    float angle = 20.f * m_elapsedTime;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, -4.2f, -5.f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(2.5f));

    glm::mat4 mvp = m_projection * m_view * model;
    m_sceneProgram->bind();
    m_sceneProgram->setUniform("mvp", mvp);
    m_sceneProgram->setUniform("model", model);
    m_boxText->bind(0);
    m_sceneProgram->setUniform("diffuseText", 0);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 8);
    
    
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(1.f, -4.2f, -8.f));
    model = glm::scale(model, glm::vec3(2.5f));
    mvp = m_projection * m_view * model;
    
    m_sceneProgram->setUniform("mvp", mvp);
    m_sceneProgram->setUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 8);

    ///////// READ
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, 640, 480, 0, 240, 320, 480, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 640, 480, 320, 240, 640, 480, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, 640, 480, 0, 0, 320, 240, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    */
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    m_quadProgram->bind();
    glBindVertexArray(m_quadVao);
    glBindTextureUnit(0, m_positionText);
    glBindTextureUnit(1, m_normalText);
    glBindTextureUnit(2, m_diffuseText);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_ebo);
    delete m_smileText;
    delete m_boxText;
    delete m_sceneProgram;
}
