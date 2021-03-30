#pragma once
#include <glad/glad.h>
#include "GLProgram.h"

class QuadTexturedDraw {
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
    float m_elapsedTime;
};