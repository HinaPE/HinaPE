#version 330 core
in vec2 f_tex_coord;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main() {
//    color = vec4(0.0f, 0.5f, 0.2f, 1.0f);
    color = texture(texture_diffuse1, f_tex_coord);
}