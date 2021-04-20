#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;

out vec2 vert_uv;

out vec3 worldNorm;
out vec3 worldPos;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);  
    vert_uv = aUv;

    worldNorm = mat3(transpose(inverse(model))) * aNorm;

    vec4 worldPos4f = model * vec4(aPos, 1.f);
    worldPos = worldPos4f.xyz;
}