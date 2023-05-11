#version 330 core

layout (location = 0) in vec2 vertPos;
layout (location = 1) in vec2 texCoord;

uniform vec2 playerPos;
uniform vec2 screenDim;
uniform float yUnits;
uniform vec2 xRot;
uniform vec2 yRot;
uniform vec2 minUV;
uniform vec2 UVDim;
uniform bool mini;

out vec2 uv;

void main() {
    uv = minUV + UVDim * texCoord;
    vec2 sizedVertPos = vertPos;
    if(mini){
        sizedVertPos = 0.5 * sizedVertPos;
    }
    vec2 rotatedVertPos = vec2(dot(xRot, sizedVertPos), dot(yRot, sizedVertPos));
    vec2 screenPos = (vec2(0, playerPos.y)+rotatedVertPos)/vec2((yUnits * screenDim.x)/screenDim.y, yUnits);
    gl_Position = vec4(screenPos, 0, 1);
}
