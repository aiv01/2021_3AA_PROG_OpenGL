#include "QuadColorDraw2.h"
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

struct color {
    float r;
    float g;
    float b;
    float a;
};

void QuadColorDraw2::start() {
    //1. Create Vertex Shader
    GLuint vertShader = createShader("resources/shaders/quadcolor2.vert", GL_VERTEX_SHADER);
    //2. Create Fragment Shader
    GLuint fragShader = createShader("resources/shaders/quadcolor2.frag", GL_FRAGMENT_SHADER);
    //3. Create Program and bind Vertex/Fragm Shader (Pipeline definition)
    m_prog = createProgram(vertShader, fragShader);

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
         0.5f, -0.5f, 0.0f,    //bottom right
        -0.5f, -0.5f, 0.0f,    //bottom left
        -0.5f,  0.5f, 0.0f,    //top left
         0.5f,  0.5f, 0.0f,    //top right
    };

    std::vector<float> colors = {
        1.f, 0.f, 0.f,          //bottom right
        0.f, 1.f, 0.f,          //bottom left
        0.f, 0.f, 1.f,          //top left
        1.f, 1.f, 0.f,          //top right
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
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //- Create Buffer to load colors data (VBO)
    glGenBuffers(1, &m_vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colors);
    dataSize = colors.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, colors.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_1);

    //- Create Element Buffer to load indices (EBO)
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    int elemSize = indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemSize, indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glUseProgram(m_prog);


    GLint colorLoc = glGetUniformLocation(m_prog, "color");
    //glUniform4f(colorLoc, 1.f, 0.f, 0.f, 1.f);

    color red = {1.f, 0.f, 0.f, 1.f};
    const GLfloat* redf = (GLfloat*)&red;
    glUniform4fv(colorLoc, 1, redf);

    m_elapsedTime = 0.f;
}

void QuadColorDraw2::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    m_elapsedTime += deltaTime;

    color c;
    c.r = sinf(m_elapsedTime) * 0.5f + 0.5f;
    c.g = cosf(m_elapsedTime) * 0.5f + 0.5f;
    c.b = cosf(m_elapsedTime + 1.1f) * 0.5f + 0.5f;
    c.a = 1.f;

    //glUniform4fv(glGetUniformLocation(m_prog, "color"), 1, (GLfloat*)&c);
    glUniform4fv(5, 1, (GLfloat*)&c);


    glUniform1f(glGetUniformLocation(m_prog, "data.value1"), 1.f);
    glUniform1f(glGetUniformLocation(m_prog, "data.value2"), 1.f);
    glUniform1f(glGetUniformLocation(m_prog, "data.value3"), 1.f);

    printf("Location: %d\n", glGetUniformLocation(m_prog, "data.value3"));
}

void QuadColorDraw2::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_vbo_colors);
    glDeleteBuffers(1, &m_ebo);
    glDeleteProgram(m_prog);
}