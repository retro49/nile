#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

out vec4 fragColor;
out vec2 texCoord;

void main() {
    texCoord = aTex;
    fragColor = vec4(aCol, 1.0f);
    gl_Position = vec4(aPos, 1.0f);
}
