#include "PostFxDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>


void PostFxDraw::start() {
    m_programScene = GLProgram::createByShaderName("postfx");

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
        //FRONT FACE
        -1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,  -1.f,  1.f,   1.f, 0.f, //bottom right
        1.f,   1.f,  1.f,   1.f, 1.f, //top right
        -1.f,  1.f,  1.f,   0.f, 1.f, //top left
        -1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f,  1.f,   1.f, 1.f, //top right

        //BACK FACE
        1.f,   -1.f,  -1.f,   0.f, 0.f, //bottom left
        -1.f,  -1.f,  -1.f,   1.f, 0.f, //bottom right
        -1.f,   1.f,  -1.f,   1.f, 1.f, //top right
        1.f,    1.f,  -1.f,   0.f, 1.f, //top left
        1.f,   -1.f,  -1.f,   0.f, 0.f, //bottom left
        -1.f,   1.f,  -1.f,   1.f, 1.f, //top right

        //LEFT FACE
        -1.f, -1.f, -1.f,   0.f, 0.f, //bottom left
        -1.f, -1.f, 1.f,    1.f, 0.f, //bottom right
        -1.f,  1.f, 1.f,    1.f, 1.f, //top right
        -1.f,  1.f, -1.f,   0.f, 1.f, //top left
        -1.f, -1.f, -1.f,   0.f, 0.f, //bottom left
        -1.f,  1.f, 1.f,    1.f, 1.f, //top right

        //RIGHT FACE
        1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f, -1.f,  -1.f,  1.f, 0.f, //bottom right
        1.f, 1.f,   -1.f,  1.f, 1.f, //top right
        1.f, 1.f,   1.f,   0.f, 1.f, //top left
        1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f, 1.f,   -1.f,  1.f, 1.f, //top right

        //TOP FACE
        -1.f,  1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f,  1.f,   1.f, 0.f, //bottom right
        1.f,   1.f, -1.f,   1.f, 1.f, //top right
        -1.f,  1.f, -1.f,   0.f, 1.f, //top left
        -1.f,  1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f, -1.f,   1.f, 1.f, //top right

        //BOTTOM FACE
        -1.f,  -1.f,  -1.f,  0.f, 0.f, //bottom left
        1.f,   -1.f,  -1.f,  1.f, 0.f, //bottom right
        1.f,   -1.f,  1.f,   1.f, 1.f, //top right
        -1.f,  -1.f,  1.f,   0.f, 1.f, //top left
        -1.f,  -1.f,  -1.f,  0.f, 0.f, //bottom left
        1.f,   -1.f,  1.f,   1.f, 1.f //top right
    };

    //- Create Vertex Array (VAO)
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //- Create Buffer to load vertex data (VBO)
    glGenBuffers(1, &m_vbo_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertex);
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //- VAO link VBO to Vertex Shader
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(location_1);

    //- Texture
    m_boxText = new GLTexture("resources/textures/wood-box.jpg");

    //5. Set Viewport
    //glViewport(0, 0, 640, 480);
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

    int width = 640;
    int height = 480;

    //1. Rendering Scena solito, solo verso il framebuffer intermedio
    glGenFramebuffers(1, &m_sceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFbo);
    // Create texture as output for COLOR attachment
    glGenTextures(1, &m_sceneTexture);
    glBindTexture(GL_TEXTURE_2D, m_sceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_sceneTexture, 0);
    // Create render buffer as output for DEPTH attachement
    glGenRenderbuffers(1, &m_sceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_sceneRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_sceneRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) DIE("Framebuffer not completed!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //2. Pipeline PostFX
    //   Quad, nel quale renderizziamo la texture con gli effetti

    m_quadProgram = GLProgram::createByShaderName("postfxquad");
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

    glViewport(0, 0, width, height);
}

void PostFxDraw::update(float deltaTime) {
    //1. Render Scene to FrameBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_elapsedTime += deltaTime;
    //program->setUniform("rotation", m_elapsedTime);
    float angle = 20.f * m_elapsedTime;

    angle = 0.f;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(2.f));

    glm::mat4 mvp = m_projection * m_view * model;
    m_programScene->bind();
    m_programScene->setUniform("mvp", mvp);
    m_boxText->bind(1);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    //2. Render Texture to Main FrameBuffer 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    m_quadProgram->bind();
    glBindVertexArray(m_quadVao);
    glBindTextureUnit(0, m_sceneTexture);
    m_quadProgram->setUniform("sceneText", 0);
    m_quadProgram->setUniform("fx", 4);
    m_quadProgram->setUniform("time", m_elapsedTime);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostFxDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteTextures(1, &m_sceneTexture);
    glDeleteRenderbuffers(1, &m_sceneRbo);
    glDeleteFramebuffers(1, &m_sceneFbo);
    delete m_boxText;
    delete m_programScene;
}
