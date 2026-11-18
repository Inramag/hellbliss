#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;

uniform vec2 viewport;

out vec4 vcolor;
out vec2 vuv;

void main() {
    vec2 clip = vec2(pos.x / viewport.x * 2.0 - 1.0, 1.0 - pos.y / viewport.y * 2.0);

    gl_Position = vec4(clip, 0.0, 1.0);
    vcolor = color;
    vuv = uv;
}
