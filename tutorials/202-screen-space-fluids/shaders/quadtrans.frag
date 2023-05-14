#version 330
uniform vec4 color;
//in vec3 vNormal;
in vec2 uv;
in vec4 position_world_space;
out vec4 out_color;
void main() {
    //float s = mix(0.1, 0.9, abs(dot(vNormal, vec3(0.0, 1.0, 0.0))));
    //out_color = vec4(s * color.rgb, color.a);
    vec3 color = vec3(0, 0, 1);
    float repeat = 10;
    float aa = .03;
    float modx = mod(position_world_space.x*repeat, 1.f);
    float modz = mod(position_world_space.z*repeat, 1.f);
    if (modx>(1.f-aa)){
        modx = (1.f-modx) + .5;
    }

    if (modx<aa){
        modx = (1.f-modx) - .5;
    }

    if (modz>(1.f-aa)){
        modz = (1.f-modz) + .5;
    }

    if (modz<aa){
        modz = (1.f-modz) - .5;
    }

    float range = 2.f*aa;

    if (modz<(.5-aa)){

        if (modx<(.5-aa)){
            color = vec3(1, 1, 1);
        }
        if (modx<(.5+aa) && modx>(.5-aa)){
            color = mix(vec3(1, 1, 1), vec3(0, 0, 1), (modx-(.5-aa))/range);
        }
    } else if (modz>(.5+aa)){
        if (modx>(.5+aa)){
            color = vec3(1, 1, 1);
        }
        if (modx<(.5+aa) && modx>(.5-aa)){
            color = mix(vec3(1, 1, 1), vec3(0, 0, 1), 1-(modx-(.5-aa))/range);
        }
    } else {
        if (modx>.5){
            color = mix(vec3(1, 1, 1), vec3(0, 0, 1), 1-(modz-(.5-aa))/range);
        } else {
            color = mix(vec3(1, 1, 1), vec3(0, 0, 1), (modz-(.5-aa))/range);
        }
    }


    //    if(int(position_world_space.x*repeat+10001)%2==1 && int(position_world_space.z*repeat+10001)%2==0){
    //        color = vec3(1,1,1);
    //    }

    out_color = vec4(color, 1);
}
