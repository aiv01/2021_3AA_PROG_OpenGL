#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

uniform mat4 mvp;

out vec2 vert_uv;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);  
    vert_uv = aUv;
}