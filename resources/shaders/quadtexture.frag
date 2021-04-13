#version 450 core
out vec4 fragColor;

in vec2 vert_uv;

layout (binding = 1) uniform sampler2D boxText;

void main() {
    vec4 color;
    color = texture(boxText, vert_uv);
    fragColor = color;
}