#version 330
uniform sampler2D tex;
in vec2 uv;
out vec4 fragColor;
uniform float near;
uniform float far;
uniform mat4 proj;
uniform mat4 view;
uniform int render_style;

void main() {
    if(render_style==1){
        //scaled to see depth
        float n = near;
        float f = far;
        float z = texture(tex,uv).x;
        z= (2.0 * n) / (f + n - z * (f - n));
        fragColor = vec4(z,z,z,1);

    }else if(render_style==0){
        fragColor = texture(tex,uv);
    }else if(render_style==2){
        fragColor = vec4(vec3(texture(tex,uv).x),1);
    }else if(render_style==3){
        fragColor = vec4(vec3(texture(tex,uv).z),1);
    }
}
