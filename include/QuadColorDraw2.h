#pragma once
#include <glad/glad.h>

class QuadColorDraw2 {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo_vertex;
    GLuint m_vbo_colors;
    GLuint m_ebo;
    GLuint m_prog;
    float m_elapsedTime;
};