#pragma once
#include <glad/glad.h>
#include "GLProgram.h"
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class DeferredDraw {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo_vertex;
    GLuint m_ebo;
    GLuint m_prog;
    GLProgram* m_sceneProgram;
    GLTexture* m_smileText;
    GLTexture* m_boxText;
    float m_elapsedTime;
    glm::mat4 m_view;
    glm::mat4 m_projection; 
    std::vector<float> m_vertices;

    GLuint m_gBufferRbo;
    GLuint m_gBuffer;
    GLuint m_positionText;
    GLuint m_normalText;
    GLuint m_diffuseText;
    GLProgram* m_quadProgram;
    GLuint m_quadVao;
    GLuint m_quadVbo;
};