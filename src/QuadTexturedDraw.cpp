#include "QuadTexturedDraw.h"
#include "Commons.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


// static GLuint createTexture(const char* path) {
//     stbi_set_flip_vertically_on_load(true);
//     int width, height, channels;
//     unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
//     DIE_ON_NULL(data, "Failed Loading texture!!!");
//     GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
//     //GLenum format = GL_RGBA;
    
//     GLuint textureId;
//     glGenTextures(1, &textureId);
//     glBindTexture(GL_TEXTURE_2D, textureId);
    
//     //Load data
//     glTexImage2D(GL_TEXTURE_2D, 0, 
//                     format, width, height, 0, 
//                     format, GL_UNSIGNED_BYTE, data);
//     //Wrapping
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     //Filtering
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//     //Optional Mipmappig.
//     glGenerateMipmap(GL_TEXTURE_2D);

//     //Destroy texture data
//     stbi_image_free(data);
//     return textureId;
// }

void QuadTexturedDraw::start() {
    program = new GLProgram("resources/shaders/quadtexture.vert", 
                            "resources/shaders/quadtexture.frag");

    //4. Load Vertex data to GPU
    std::vector<float> vertices = {
         0.5f, -0.5f, 0.0f,   1.f, 0.f, //bottom right
        -0.5f, -0.5f, 0.0f,   0.f, 0.f, //bottom left
        -0.5f,  0.5f, 0.0f,   0.f, 1.f, //top left
         0.5f,  0.5f, 0.0f,   1.f, 1.f //top right
    };

    std::vector<uint32_t> indexes = {
        0, 1, 2,  //left triangle
        2, 3, 0   //right triangle
    };

    //- Create Vertex Array (VAO)
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //- Create Buffer to load vertex data (VBO)
    glGenBuffers(1, &m_vbo_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertex);
    int dataSize = vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    //- VAO link VBO to Vertex Shader
    GLuint location_0 = 0;
    glVertexAttribPointer(location_0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(location_0);

    //- VAO link VBO to Vertex Shader
    GLuint location_1 = 1;
    glVertexAttribPointer(location_1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(location_1);

    //- Create Element Buffer to load indices (EBO)
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    int elemSize = indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemSize, indexes.data(), GL_STATIC_DRAW);

    //- Texture
    // GLuint smileId = createTexture("resources/textures/smile.png");
    // GLuint boxId = createTexture("resources/textures/wood-box.jpg");
    m_smileText = new GLTexture("resources/textures/smile.png");
    m_boxText = new GLTexture("resources/textures/wood-box.jpg");

    //5. Set Viewport
    glViewport(0, 0, 640, 480);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    program->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program->setUniform("smileText", 0);
    //Avoid set uniform with this config: 
    //    layout (binding = 1) uniform sampler2D boxText;
    //program->setUniform("boxText", 1);
    
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, smileId);

    m_smileText->bind(GL_TEXTURE0);
    m_boxText->bind(GL_TEXTURE1);
    //glActiveTexture(GL_TEXTURE1);
    // glActiveTexture(GL_TEXTURE0 + 1);
    // glBindTexture(GL_TEXTURE_2D, boxId);

    program->setUniform("selectSmile", false);

    m_elapsedTime = 0.f;
}

void QuadTexturedDraw::update(float deltaTime) {
    //6. Draw Call
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    m_elapsedTime += deltaTime;

}

void QuadTexturedDraw::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_vertex);
    glDeleteBuffers(1, &m_ebo);
    delete m_smileText;
    delete m_boxText;
    delete program;
}
