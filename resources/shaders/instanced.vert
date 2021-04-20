#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 aMvp;

//uniform mat4 mvp[10];

void main() {
    //gl_InstanceID
    //mvp[gl_InstanceID]
    //gl_Position = mvp * vec4(aPos + gl_InstanceID, 1.0);
    gl_Position = aMvp * vec4(aPos, 1.0);  
}