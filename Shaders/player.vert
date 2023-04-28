#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 playerPos;
uniform vec2 screenDim;
uniform float theta;

void main() {
    vec2 newVertPos = vec2(dot(vec2(cos(theta), sin(theta)), vertPos), dot(vec2(-sin(theta), cos(theta)), vertPos));
    vec2 screenPos = (vec2(0, playerPos.y)+newVertPos)/screenDim;
    gl_Position = vec4(screenPos, 0, 1);
}
