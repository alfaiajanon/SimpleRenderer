#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCord;

out vec2 texCord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model*vec4(aPos, 1.0f);
    fragPos= (model*vec4(aPos,1.0f)).xyz;
    texCord=aTexCord;
    normal=aNormal;
}