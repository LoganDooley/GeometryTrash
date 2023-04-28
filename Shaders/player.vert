#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 playerPos;
uniform vec2 screenDim;
uniform vec2 xRot;
uniform vec2 yRot;

void main() {
    vec2 rotatedVertPos = vec2(dot(xRot, vertPos), dot(yRot, vertPos));
    vec2 screenPos = (vec2(0, playerPos.y)+rotatedVertPos)/screenDim;
    gl_Position = vec4(screenPos, 0, 1);
}
