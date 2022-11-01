#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in uint v_id;

uniform mat4 mvp, normal;

smooth out vec3 f_norm;
flat out uint f_id;

void main() {
    f_id = v_id;
    f_norm = (normal * vec4(v_norm, 0.0f)).xyz;
    gl_Position = mvp * vec4(v_pos, 1.0f);
}