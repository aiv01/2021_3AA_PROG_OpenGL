#include "GLProgram.h"
#include "Commons.h"
#include <vector>
#include <iostream>
#include <fstream>

static std::vector<char> readFile(const std::string &filePath)
{
    //std::cout << std::filesystem::current_path() << std::endl;
    std::ifstream file(filePath, std::ios::ate);
    if (!file.is_open())
        DIE("failed opening file!!!");

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

static GLuint createShader(const std::string &filePath, GLenum shaderType)
{
    std::vector<char> shaderStr = readFile(filePath);
    const char *shaderSource = shaderStr.data();

    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, NULL);
    glCompileShader(shaderId);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        DIE(infoLog);
    }
    return shaderId;
}

static GLuint createProgram(GLuint vertId, GLuint fragId)
{
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertId);
    glAttachShader(programId, fragId);
    glLinkProgram(programId);

    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        DIE(infoLog);
    }

    glDeleteShader(vertId);
    glDeleteShader(fragId);
    return programId;
}

GLProgram::GLProgram(const std::string &vertPath, const std::string &fragPath)
{
    //1. Create Vertex Shader
    GLuint vertShader = createShader(vertPath, GL_VERTEX_SHADER);
    //2. Create Fragment Shader
    GLuint fragShader = createShader(fragPath, GL_FRAGMENT_SHADER);

    m_prog = createProgram(vertShader, fragShader);
}

GLuint GLProgram::getId() const{
    return m_prog;
}

void GLProgram::bind() const{
    glUseProgram(m_prog);
}

void GLProgram::setUniform(const std::string& name, const color& c){
    GLint colorLoc = glGetUniformLocation(m_prog, (GLchar*)name.c_str());

    const GLfloat* cf = (GLfloat*)&c;
    glUniform4fv(colorLoc, 1, cf);
}

void GLProgram::setUniform(const std::string& name, bool value) {
    GLint loc = glGetUniformLocation(m_prog, (GLchar*)name.c_str());
    if (value) glUniform1i(loc, 1);
    else glUniform1i(loc, 0);
}

void GLProgram::setUniform(const std::string& name, int value) {
    GLint loc = glGetUniformLocation(m_prog, (GLchar*)name.c_str());
    glUniform1i(loc, value);
}

void GLProgram::setUniform(const std::string& name, float value) {
    GLint loc = glGetUniformLocation(m_prog, (GLchar*)name.c_str());
    glUniform1f(loc, value);
}

void GLProgram::setUniform(const std::string& name, const glm::mat4& value) {
    GLint loc = glGetUniformLocation(m_prog, (GLchar*)name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

GLProgram::~GLProgram(){
    glDeleteProgram(m_prog);
}

GLProgram* GLProgram::createByShaderName(const std::string& shaderName){
    std::string vertPath = "resources/shaders/"+shaderName+".vert";
    std::string fragPath = "resources/shaders/"+shaderName+".frag";
    GLProgram* gl_prog = new GLProgram(vertPath, fragPath);
    return gl_prog;
}

