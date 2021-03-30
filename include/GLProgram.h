#pragma once

#include <string>
#include <glad/glad.h>

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

    GLuint getId() const;
    void bind() const;
    void setUniform(const std::string& name, const color& c);

    ~GLProgram();

private:
    GLuint m_prog;
};