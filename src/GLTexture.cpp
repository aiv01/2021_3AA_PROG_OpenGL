#include "GLTexture.h"
#include "Commons.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint createTexture(const char* path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    DIE_ON_NULL(data, "Failed Loading texture!!!");
    GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
    //GLenum format = GL_RGBA;
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    //Load data
    glTexImage2D(GL_TEXTURE_2D, 0, 
                    format, width, height, 0, 
                    format, GL_UNSIGNED_BYTE, data);
    //Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Optional Mipmappig.
    glGenerateMipmap(GL_TEXTURE_2D);

    //Destroy texture data
    stbi_image_free(data);
    return textureId;
}

GLTexture::GLTexture(const std::string& imgPath){
    m_textureId = createTexture(imgPath.c_str());
}

GLTexture::~GLTexture(){
    glDeleteTextures(1, &m_textureId);
}

void GLTexture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}
