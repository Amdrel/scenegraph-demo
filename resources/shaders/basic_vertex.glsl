#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 uv;

uniform mat4 transform;

void main() {
    uv = aUv;
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
