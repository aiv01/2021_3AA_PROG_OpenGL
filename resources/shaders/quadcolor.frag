#version 450 core
out vec4 fragColor;

in vec4 base_color;
in vec4 vert_color;

void main() {
    //fragColor = vec4(1.f, 0.f, 0.f, 1.f);
    //fragColor = base_color * vert_color;
    fragColor = mix(base_color, vert_color, 0.2f);
}