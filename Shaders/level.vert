#version 330 core

layout (location = 0) in vec2 vertPos;

uniform vec2 objPos;
uniform vec2 playerPos;
uniform vec2 screenDim;
uniform float yUnits;
uniform bool flipX;
uniform bool flipY;

void main() {
    vec2 newVertPos = vertPos;
    if(flipX){
        newVertPos.y = -newVertPos.y;
    }
    if(flipY){
        newVertPos.x = -newVertPos.x;
    }
    vec2 screenPos = (objPos + newVertPos - vec2(playerPos.x, 0))/vec2((yUnits * screenDim.x)/screenDim.y, yUnits);
    gl_Position = vec4(screenPos, 0, 1);
}
