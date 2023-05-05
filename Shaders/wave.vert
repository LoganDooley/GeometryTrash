#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 playerPos;
uniform vec2 screenDim;
uniform float yUnits;

void main() {
    vec2 screenPos = (vertPos - vec2(playerPos.x, 0))/vec2((yUnits * screenDim.x)/screenDim.y, yUnits);
    gl_Position = vec4(screenPos, 0, 1);
}
