#version 450 core
out vec4 fragColor;

in vec3 vert_uv;

uniform samplerCube cubemap;

void main() {
    vec3 uv = vert_uv;
    uv.z *= -1.f;
    fragColor = texture(cubemap, uv);
}