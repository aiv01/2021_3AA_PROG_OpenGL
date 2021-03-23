#include "TriangleDraw.h"
#include <vector>
#include <string>
#include <fstream>
#include "Commons.h"
#include <filesystem>
#include <iostream>
#include <glad/glad.h>

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

void TriangleDraw::start() {
    //1. Create Vertex Shader
    GLuint vertShader = createShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);
    //2. Create Fragment Shader
    GLuint fragShader = createShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);
    //3. Create Program and bind Vertex/Fragm Shader (Pipeline definition)
    GLuint progId = createProgram(vertShader, fragShader);

    //4. Load Vertex data to GPU

    //5. Set Viewport
}

void TriangleDraw::update() {
    //6. Draw Call
}