#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
out vec2 f_tex_coord;
out vec4 f_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

void main() {
    gl_Position = projection * model * vec4(position.x, position.y, 0.0, 1.0);
    f_tex_coord = tex_coord;
    f_color = color;
}