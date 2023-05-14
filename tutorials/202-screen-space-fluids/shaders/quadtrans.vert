#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 inUV;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
out vec3 vNormal;
out vec2 uv;
out vec4 position_world_space;
void main() {
    uv = inUV;
    gl_Position = p*v*m* vec4(position, 1.0);
    position_world_space = m*vec4(position, 1.0);
    //vNormal = normal;
}
