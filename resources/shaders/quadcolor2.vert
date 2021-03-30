#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

layout (location = 1) out vec4 vert_color;

struct mydata_t {
    float value1;
    float value2;
    float value3;
};

layout (location = 1) uniform mydata_t data;

//

void main() {
    gl_Position = vec4(aPos, 1.0);
    vert_color = vec4(aColor, 1.f) * data.value1 * data.value2 * data.value3;
}