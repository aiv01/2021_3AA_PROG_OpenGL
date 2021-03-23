#pragma once
#include <glad/glad.h>

class QuadIndexDraw {
public:
    void start();
    void update();
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_prog;
};