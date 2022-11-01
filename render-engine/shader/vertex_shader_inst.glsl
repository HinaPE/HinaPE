#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in uint v_id;

layout (location = 3) in uint i_id;
layout (location = 4) in mat4 i_trans;

uniform bool use_i_id;
uniform mat4 proj, modelview;

smooth out vec3 f_norm;
flat out uint f_id;

void main() {
    f_id = use_i_id ? i_id : v_id;
    mat4 mv = modelview * i_trans;
    mat4 n = transpose(inverse(mv));
    f_norm = (n * vec4(v_norm, 0.0f)).xyz;
    gl_Position = proj * mv * vec4(v_pos, 1.0f);
}