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
    program = GLProgram::createByShaderName("deferredgbuffer");

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
    program->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    m_boxText->bind(1);

    m_elapsedTime = 0.f;

    //camera
    glm::vec3 position = glm::vec3(0.f, 0.f, 5.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    float fovY = 60.f;
    float aspectRatio = 640.f / 480.f;
    float zNear = 0.1f;
    float zFar = 100.f;


    //model = translate * rotate * scale
    /*
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
    model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(2.f));
    */

    //glm::mat4 modelRotate = glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));

    m_view = glm::lookAt(position, position + direction, up);
    m_projection = glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar);

    //glm::mat4 mvp = projection * view * model;
    //program->setUniform("mvp", mvp);

    glm::vec3 pointLightPos = glm::vec3(10.f, 0.f, 0.f);
    program->setUniform("pointLightPos", pointLightPos);
    program->setUniform("cameraPos", position);

    glClearColor(0.f, 0.f, 0.f, 1.f);
}

void DeferredDraw::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    m_elapsedTime += deltaTime;
    float angle = 20.f * m_elapsedTime;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, -4.2f, -5.f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(2.5f));

    glm::mat4 mvp = m_projection * m_view * model;
    program->setUniform("mvp", mvp);
    program->setUniform("model", model);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 8);
}

void DeferredDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_ebo);
    delete m_smileText;
    delete m_boxText;
    delete program;
}
