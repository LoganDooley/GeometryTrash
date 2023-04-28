#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 objPos;
uniform vec2 playerPos;
uniform vec2 screenDim;

void main() {
    vec2 screenPos = (objPos + vertPos - vec2(playerPos.x, 0))/screenDim;
    gl_Position = vec4(screenPos, 0, 1);
}
