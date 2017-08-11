#version 330 core

in vec2 uv;
out vec4 fragColor;

uniform sampler2D texture0;

void main() {
    fragColor = texture(texture0, vec2(uv.x, 1.0 - uv.y));
}
