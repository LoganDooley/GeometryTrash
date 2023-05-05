#version 330 core

out vec4 fragColor;

uniform sampler2D iconTexture;
uniform vec3 playerColor1;
uniform vec3 playerColor2;

in vec2 uv;

void main() {
    vec4 texColor = texture(iconTexture, uv);
    if(texColor.a < 0.1){
        discard;
    }
    else if(texColor.r < 0.1){
        texColor = vec4(0, 0, 0, texColor.a);
    }
    else if(texColor.r > 0.9){
        texColor = vec4(playerColor1, texColor.a);
    }
    else{
        texColor = vec4(playerColor2, texColor.a);
    }
    fragColor = texColor;
}
