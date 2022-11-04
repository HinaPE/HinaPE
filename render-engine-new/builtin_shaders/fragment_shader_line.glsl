#version 330 core

layout (location = 0) out vec4 out_col;
layout (location = 1) out vec4 out_id;

uniform float alpha;
smooth in vec3 f_col;

void main() {
    out_id = vec4(0.0f);
    out_col = vec4(f_col, alpha);
}