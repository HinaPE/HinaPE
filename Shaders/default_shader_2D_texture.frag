#version 330 core
in vec4 f_color;
in vec2 f_tex_coord;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main() {
    color = texture(texture_diffuse1, f_tex_coord);
}