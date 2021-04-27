#version 450 core
layout (location = 0) out vec3 gPosition; //GL_COLOR_ATTACHMENT0
layout (location = 1) out vec3 gNormal; //GL_COLOR_ATTACHMENT1
layout (location = 2) out vec3 gDiffuse; //GL_COLOR_ATTACHMENT2

in vec2 vert_uv;
in vec3 worldNorm;
in vec3 worldPos;

uniform sampler2D diffuseText;

void main() {
    gPosition = worldPos;
    gNormal = normalize(worldNorm);
    gDiffuse = texture(diffuseText, vert_uv).rgb;
    //gPosition = gDiffuse;
}