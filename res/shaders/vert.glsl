#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 viewProj;

out vec2 TexCoord;

void main() {
    gl_Position = viewProj * model * vec4(inPos.x, inPos.y, inPos.z, 1.0);
    TexCoord = inTexCoord;
}