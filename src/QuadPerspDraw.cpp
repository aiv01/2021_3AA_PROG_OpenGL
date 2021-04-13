#include "QuadPerspDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

void QuadPerspDraw::start() {
    program = GLProgram::createByShaderName("quadpersp");

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
         0.5f, -0.5f, 0.0f,   1.f, 0.f, //bottom right
        -0.5f, -0.5f, 0.0f,   0.f, 0.f, //bottom left
        -0.5f,  0.5f, 0.0f,   0.f, 1.f, //top left
         0.5f,  0.5f, 0.0f,   1.f, 1.f //top right
    };

    std::vector<uint32_t> indexes = {
        0, 1, 2,  //left triangle
        2, 3, 0   //right triangle
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

    //- Create Element Buffer to load indices (EBO)
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    int elemSize = indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemSize, indexes.data(), GL_STATIC_DRAW);

    //- Texture
    m_boxText = new GLTexture("resources/textures/wood-box.jpg");

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    program->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_boxText->bind(1);

    m_elapsedTime = 0.f;
}

void QuadPerspDraw::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    m_elapsedTime += deltaTime;

}

void QuadPerspDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_ebo);
    delete m_smileText;
    delete m_boxText;
    delete program;
}
