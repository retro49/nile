#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

out vec4 fragColor;
out vec2 texCoord;
uniform float u_time;

void main() {
    texCoord = aTex;
    fragColor = vec4(aCol, 1.0f);

    vec4 modPos = vec4(aPos, 1.0f);
    modPos.x = sin(u_time * 5) / 2;

    gl_Position = modPos;
}
