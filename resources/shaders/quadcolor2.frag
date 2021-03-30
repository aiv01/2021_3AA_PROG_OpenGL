#version 450 core
out vec4 fragColor;

layout (location = 5) uniform vec4 color;

layout (location = 1) in vec4 frag_color;

void main() {
    //fragColor = vec4(1.f, 0.f, 0.f, 1.f);
    //fragColor = base_color * vert_color;
    //fragColor = mix(base_color, vert_color, 0.2f);
    fragColor = mix(color, frag_color, 0.2f);
}