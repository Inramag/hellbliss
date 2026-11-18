#version 330 core

in vec4 vcolor;
in vec2 vuv;

uniform sampler2D tex;
uniform bool usetex;

out vec4 FragColor;

void main() {
    FragColor = vcolor;

    if (usetex) FragColor *= texture(tex, vuv);
}
