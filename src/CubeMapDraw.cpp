#include "CubeMapDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>


void CubeMapDraw::start() {
    program = GLProgram::createByShaderName("cubemap");

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
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
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

}

void CubeMapDraw::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_elapsedTime += deltaTime;
    //program->setUniform("rotation", m_elapsedTime);
    float angle = 20.f * m_elapsedTime;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
        //glm::quat quat = glm::quat(glm::vec3(0.f, glm::radians(angle), 0.f));
        glm::quat quat = glm::angleAxis(glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 rotation = glm::toMat4(quat);
        model = model * rotation;
    model = glm::scale(model, glm::vec3(2.f));

    glm::mat4 mvp = m_projection * m_view * model;
    program->setUniform("mvp", mvp);
}

void CubeMapDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_ebo);
    delete m_smileText;
    delete m_boxText;
    delete program;
}
