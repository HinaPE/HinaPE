#version 330 core

#define PI 3.1415926535f
#define TAU 6.28318530718f

uniform vec3 color;
uniform float cosine;
uniform bool use_texture;
uniform sampler2D tex;

layout (location = 0) out vec4 out_col;
layout (location = 1) out vec4 out_id;

smooth in vec3 f_pos;

void main() {
    vec3 pos = normalize(f_pos);
    if(pos.y > cosine) {
        if(use_texture) {
            float theta = atan(pos.z,pos.x) / TAU;
            float phi = acos(pos.y) / PI;
            out_col = texture(tex, vec2(theta,phi));
        } else {
            out_col = vec4(color, 1.0f);
        }
    }
    else discard;
}