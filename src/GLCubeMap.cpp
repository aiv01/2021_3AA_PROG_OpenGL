#include "GLCubeMap.h"
#include "Commons.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint createTexture(const std::vector<std::string>& imgPaths) {
    //stbi_set_flip_vertically_on_load(true);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    
    /*
        GL_TEXTURE_CUBE_MAP_POSITIVE_X  right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X  left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y  top 
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y  bottom
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z  back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  front
    */

    for(int i=0; i<6; i++) {
        const std::string& path = imgPaths[i];
        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        DIE_ON_NULL(data, "Failed Loading texture!!!");
        GLenum format = channels == 3 ? GL_RGB : GL_RGBA;

        //Load data
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                    0, format, width, height, 0, 
                        format, GL_UNSIGNED_BYTE, data);
        //Destroy texture data
        stbi_image_free(data);
    }

    //Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureId;
}

GLCubeMap::GLCubeMap(const std::vector<std::string>& imgPaths) {
    m_textureId = createTexture(imgPaths);
}

GLCubeMap::~GLCubeMap(){
    glDeleteTextures(1, &m_textureId);
}

void GLCubeMap::bind(GLuint textureUnit0based) {
    glBindTextureUnit(textureUnit0based, m_textureId);
}
