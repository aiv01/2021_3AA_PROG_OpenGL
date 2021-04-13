#pragma once
#include <string>
#include <glad/glad.h>

class GLTexture{
    public:
    GLTexture(const std::string& imgPath);
    ~GLTexture();
    void bind(GLuint textureUnit0based);
    void bindLegacy(GLenum textureUnitEnum);

    private:
    GLuint m_textureId;
};
    