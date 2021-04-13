#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform float rotation;

out vec2 vert_uv;

vec3 translate(vec3 pos){
    vec3 translatedPos;
    translatedPos = pos + vec3(0.11,0.15, 0);
    return translatedPos;
}

vec3 scale(vec3 pos, float scale){
    return pos*scale;
}

vec3 rotateOnY(vec3 pos, float dgr){
    vec3 rotatedPos = pos;
    float rads = radians(dgr);
    rotatedPos.x = cos(rads) * pos.x - sin(rads) * pos.z;
    rotatedPos.z = sin(rads) * pos.x + cos(rads) * pos.z;
    return rotatedPos;
}

vec3 perspective(vec3 pos){
    vec3 perspPos;

    //calculate Aspect ratio and FOV
    float aspectRatio = 640.f/480.f;
    float halfFov = 60.f / 2.f;
    float fov = tan(radians(halfFov));

    float zNear = 0.1;
    float zFar = 1000;
    float zRangeRaw = zFar - zNear;
    float zRange = 2; 

    float reverseZ = -pos.z;
    //we are already in NDC so we dont need to translate to that space here..
    perspPos.x = pos.x / reverseZ;
    perspPos.y = pos.y / reverseZ;
    perspPos.z = pos.z;

    perspPos.x = perspPos.x / (fov * aspectRatio);
    perspPos.y = perspPos.y / fov;
    perspPos.z = ((pos.z - zNear) / zRangeRaw) * zRange + 1;

    return perspPos;
}

void main() {
    vec3 translatedPos = aPos;
    translatedPos = scale(translatedPos, 100);
    translatedPos = rotateOnY(translatedPos, rotation * 20);
    translatedPos.z -= 400;
    vec3 perspPos = perspective(translatedPos);
    gl_Position = vec4(perspPos, 1.0);  
    vert_uv = aUv;
}