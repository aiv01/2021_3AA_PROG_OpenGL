#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 vert_uv;

vec3 translate(vec3 pos){
    vec3 translatedPos;
    translatedPos = pos + vec3(0.11,0.15, 0);
    return translatedPos;
}

vec3 perspective(vec3 pos){
    vec3 perspPos;

    //calculate Aspect ratio and FOV
    float aspectRatio = 640.f/480.f;
    float halfFov = 60.f / 2.f;
    float fov = tan(radians(halfFov));

    //float reverseZ = -pos.z;
    //we are already in NDC so we dont need to translate to that space here..
    perspPos.x = pos.x;
    perspPos.y = pos.y;
    perspPos.z = pos.z;

    perspPos.x = perspPos.x / (fov * aspectRatio);
    perspPos.y = perspPos.y / fov;

    return perspPos;
}

void main() {
    vec3 translatedPos = translate(aPos);
    vec3 perspPos = perspective(translatedPos);
    gl_Position = vec4(perspPos, 1.0);
    vert_uv = aUv;
}