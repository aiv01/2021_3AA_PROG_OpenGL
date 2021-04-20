#pragma once
#include <glad/glad.h>
#include "GLProgram.h"
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct MyInstance {
    glm::vec3 position;
    float scale;
    float rotation;
};


class InstancedDraw {
public:
    void start();
    void update(float deltaTime);
    void destroy();
private:
    GLuint m_vao;
    GLuint m_vbo_vertex;
    GLuint m_prog;
    GLProgram* program;
    float m_elapsedTime;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    std::vector<MyInstance> instances;
    GLuint m_vbo_mvp;
};