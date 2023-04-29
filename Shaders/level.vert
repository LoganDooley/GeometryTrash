#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 objPos;
uniform vec2 playerPos;
uniform vec2 screenDim;
uniform bool flipX;
uniform bool flipY;

void main() {
    vec2 newVertPos = vertPos;
    if(flipX){
        newVertPos.x = -newVertPos.x;
    }
    if(flipY){
        newVertPos.y = -newVertPos.y;
    }
    vec2 screenPos = (objPos + newVertPos - vec2(playerPos.x, 0))/screenDim;
    gl_Position = vec4(screenPos, 0, 1);
}
