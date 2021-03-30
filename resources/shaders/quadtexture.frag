#version 450 core
out vec4 fragColor;

in vec2 vert_uv;

uniform sampler2D smileText;
layout (binding = 1) uniform sampler2D boxText;

uniform int selectSmile;

void main() {
    vec4 color;
    //if (selectSmile == 1) color = texture(smileText, vert_uv);
    //else color = texture(boxText, vert_uv);
    color = mix(texture(smileText, vert_uv), texture(boxText, vert_uv), 1-selectSmile);
    
    //color = mix(texture(smileText, vert_uv), texture(boxText, vert_uv), 0.5f);
    //color = texture(boxText, vert_uv);
    
    fragColor = color;
}