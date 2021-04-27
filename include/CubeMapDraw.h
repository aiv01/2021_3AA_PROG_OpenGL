#pragma once
#include <glad/glad.h>
#include "GLProgram.h"
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLCubeMap.h"

class CubeMapDraw {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo_vertex;
    GLuint m_ebo;
    GLuint m_prog;
    GLProgram* program;
    GLTexture* m_smileText;
    GLCubeMap* m_texture;
    float m_elapsedTime;
    glm::mat4 m_view;
    glm::mat4 m_projection; 

    GLProgram* m_cubeProgram;
};