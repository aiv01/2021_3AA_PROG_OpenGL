#include "InstancedDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>


void InstancedDraw::start() {
    program = GLProgram::createByShaderName("instanced");

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
       -0.5f, -0.5f, 0.0f,  //bottom-left
        0.5f, -0.5f, 0.0f,  //bottom-right
       -0.5f,  0.5f, 0.0f,  //top-left

       -0.5f,  0.5f, 0.0f,  //top-left
        0.5f, -0.5f, 0.0f,  //bottom-right
        0.5f,  0.5f, 0.0f,  //top-right
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
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    program->bind();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

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

    /*
    MyInstance p1;
    p1.position = glm::vec3(-2.f, 1.f, -3.f);
    p1.scale = 2.f;
    p1.rotation = 45.f;

    MyInstance p2;
    p2.position = glm::vec3(2.f, 1.f, -3.f);
    p2.scale = 3.f;
    p2.rotation = 15.f;

    instances.push_back(p1);
    instances.push_back(p2);
    */

    for(int i=0; i<1000; i++) {
        int row = i / 20;
        int col = i % 20;
        int sign = col < 10 ? -1 : 1;

        MyInstance p;
        p.position = glm::vec3(sign * col + 1.f, sign * row, -30.f);
        p.scale = 2.f;
        p.rotation = 1 + i * 20;
        instances.push_back(p);
    }


    //Buffer per MVP Instanced
    glGenBuffers(1, &m_vbo_mvp);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_mvp);
    glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location = 1;
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribDivisor(location, 1);

    location++;
    glEnableVertexAttribArray(location); 
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1*sizeof(glm::vec4)));
    glVertexAttribDivisor(location, 1);

    location++;
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2*sizeof(glm::vec4)));
    glVertexAttribDivisor(location, 1);

    location++;
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3*sizeof(glm::vec4)));
    glVertexAttribDivisor(location, 1);
}

void InstancedDraw::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_elapsedTime += deltaTime;
    //float angle = 20.f * m_elapsedTime;
    
    /*
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3(2.f));

    glm::mat4 mvp = m_projection * m_view * model;
    program->setUniform("mvp", mvp);
    */

    //static bool once = true;
    //if (once) {
    //    once = false;
    for(int i=0; i<instances.size(); i++) {
        MyInstance& p = instances[i];
        p.rotation += 20.f * deltaTime;

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, p.position);
        model = glm::rotate(model, glm::radians(p.rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, glm::vec3(p.scale));

        glm::mat4 mvp = m_projection * m_view * model;

        GLintptr offset = i * sizeof(glm::mat4);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::mat4), &mvp[0][0]);
    }
    //}

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instances.size());
}

void InstancedDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    delete program;
}
