#version 330 core

out vec4 finalColor;

in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    finalColor = mix(
            texture(texture1, texCoord), 
            texture(texture2, texCoord), 0.2);
}
