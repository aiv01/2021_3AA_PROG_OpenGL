#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct color {
    float r;
    float g;
    float b;
    float a;
};

class GLProgram
{
public:
    GLProgram(const std::string& vertPath,const std::string& fragPath);
    static GLProgram* createByShaderName(const std::string& shaderName);
    GLuint getId() const;
    void bind() const;
    void setUniform(const std::string& name, const color& c);
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::mat4& value);
    void setUniform(const std::string& name, const glm::vec3& value);

    ~GLProgram();

private:
    GLuint m_prog;
};