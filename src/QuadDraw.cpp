#include "QuadDraw.h"
#include <vector>
#include <string>
#include <fstream>
#include "Commons.h"
#include <filesystem>
#include <iostream>

static std::vector<char> readFile(const std::string& filePath) {
    //std::cout << std::filesystem::current_path() << std::endl;
    std::ifstream file(filePath, std::ios::ate);
    if (!file.is_open()) DIE("failed opening file!!!");
    
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

static GLuint createShader(const std::string& filePath, GLenum shaderType) {
    std::vector<char> shaderStr = readFile(filePath);
    const char* shaderSource = shaderStr.data();

    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, NULL);
    glCompileShader(shaderId);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        DIE(infoLog);
    }
    return shaderId;
}

static GLuint createProgram(GLuint vertId, GLuint fragId) {
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertId);
    glAttachShader(programId, fragId);
    glLinkProgram(programId);

    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        DIE(infoLog);
    }

    glDeleteShader(vertId);
    glDeleteShader(fragId);
    return programId;
}

void QuadDraw::start() {
    //1. Create Vertex Shader
    GLuint vertShader = createShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);
    //2. Create Fragment Shader
    GLuint fragShader = createShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);
    //3. Create Program and bind Vertex/Fragm Shader (Pipeline definition)
    m_prog = createProgram(vertShader, fragShader);

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
         0.5f, -0.5f, 0.0f,   //bottom right
        -0.5f, -0.5f, 0.0f,   //bottom left
        -0.5f,  0.5f, 0.0f,   //top left

        -0.5f,  0.5f, 0.0f,   //top left
         0.5f,  0.5f, 0.0f,   //top right
         0.5f, -0.5f, 0.0f,   //bottom right
    };

    //- Create Vertex Array (VAO)
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //- Create Buffer to load data (VBO)
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glUseProgram(m_prog);
}

void QuadDraw::update() {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glPointSize(20);
    //glDrawArrays(GL_LINE_LOOP, 0, 6);
    //glDrawArrays(GL_POINTS, 0, 6);
}

void QuadDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_prog);
}