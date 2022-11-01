#version 330 core

uniform bool solid, use_v_id;
uniform float alpha;
uniform uint id, sel_id, hov_id, err_id;
uniform vec3 color, sel_color, hov_color, err_color;

layout (location = 0) out vec4 out_col;
layout (location = 1) out vec4 out_id;

smooth in vec3 f_norm;
flat in uint f_id;

void main() {

    vec3 use_color;
    if(use_v_id) {
        out_id = vec4((f_id & 0xffu) / 255.0f, ((f_id >> 8) & 0xffu) / 255.0f, ((f_id >> 16) & 0xffu) / 255.0f, 1.0f);
        if(f_id == sel_id) {
            use_color = sel_color;
        } else if(f_id == hov_id) {
            use_color = hov_color;
        } else if (f_id == err_id) {
            use_color = err_color;
        } else {
            use_color = color;
        }
    } else {
        out_id = vec4((id & 0xffu) / 255.0f, ((id >> 8) & 0xffu) / 255.0f, ((id >> 16) & 0xffu) / 255.0f, 1.0f);
        if(id == sel_id) {
            use_color = sel_color;
        } else if(id == hov_id) {
            use_color = hov_color;
        } else if (id == err_id) {
            use_color = err_color;
        } else {
            use_color = color;
        }
    }

    if(solid) {
        out_col = vec4(use_color, alpha);
    } else {
        float ndotl = abs(normalize(f_norm).z);
        float light = clamp(0.3f + 0.6f * ndotl, 0.0f, alpha);
        out_col = vec4(light * use_color, alpha);
    }
}