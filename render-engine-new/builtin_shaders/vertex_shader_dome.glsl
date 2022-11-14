#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in uint v_id;

uniform mat4 transform;
smooth out vec3 f_pos;

void main() {
    f_pos = v_pos;
    vec4 pos = transform * vec4(v_pos, 1.0f);
    gl_Position = vec4(pos.xy, 0.000000001f, pos.w);
}