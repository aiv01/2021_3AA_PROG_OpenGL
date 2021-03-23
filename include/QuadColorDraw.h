#pragma once
#include <glad/glad.h>

class QuadColorDraw {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_prog;
    float m_elapsedTime;
};