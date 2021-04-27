#pragma once
#include <string>
#include <glad/glad.h>
#include <vector>

class GLCubeMap {
    public:
    GLCubeMap(const std::vector<std::string>& imgPaths);
    ~GLCubeMap();
    void bind(GLuint textureUnit0based);
    
    private:
    GLuint m_textureId;
};
    