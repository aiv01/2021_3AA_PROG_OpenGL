#pragma once
#include <string>
#include <glad/glad.h>

class GLTexture{
    public:
    GLTexture(const std::string& imgPath);
    ~GLTexture();
    void bind(GLenum textureUnit);

    private:
    GLuint m_textureId;
};
    