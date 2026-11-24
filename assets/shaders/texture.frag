#version 330 core

out vec4 finalColor;

in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texture1;

void main() {
    finalColor = texture(texture1, texCoord);
}
