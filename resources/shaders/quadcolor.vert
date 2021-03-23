#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec4 color;

out vec4 base_color;
out vec4 vert_color;

void main() {
    gl_Position = vec4(aPos, 1.0);
    base_color = color;
    vert_color = vec4(aColor, 1.f);
}