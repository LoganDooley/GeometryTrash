#version 330 core

out vec4 fragColor;

uniform vec3 waveColor;

void main() {
    fragColor = vec4(waveColor, 1);
}
