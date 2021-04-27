#pragma once
#include <glad/glad.h>
#include "GLProgram.h"
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PostFxDraw {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo_vertex;
    GLuint m_ebo;
    GLuint m_prog;
    GLProgram* m_programScene;
    GLTexture* m_smileText;
    GLTexture* m_boxText;
    float m_elapsedTime;
    glm::mat4 m_view;
    glm::mat4 m_projection; 

    GLuint m_sceneFbo;
    GLuint m_sceneTexture;
    GLuint m_sceneRbo;

    GLuint m_quadVao;
    GLuint m_quadVbo;
    GLProgram* m_quadProgram;
};