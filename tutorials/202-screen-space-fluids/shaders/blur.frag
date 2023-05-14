#version 400 core

in vec2 uv;

uniform sampler2D tex;


out vec4 fragColor;

uniform int horiz;

uniform float weight[11] = float[] (0.100346, 0.097274, 0.088613, 0.075856, 0.061021, 0.046128, 0.032768, 0.021874, 0.013722, 0.008089, 0.004481);


void main(){
    int width=11;
    vec2 texelSize = 1.0 / textureSize(tex, 0).xy;
    vec3 total=texture(tex, uv).xyz * weight[0];
    if (horiz>0){
        for (int i = 1;i<width;i++){ //uv.y-width*texelSize.y;i<uv.y+width*texelSize.y;i+=texelSize.y){
            total += texture(tex, vec2(uv.x, uv.y + i*texelSize.y)).xyz * weight[i];
            total += texture(tex, vec2(uv.x, uv.y - i*texelSize.y)).xyz * weight[i];
        }
    } else {
        for (int i = 1;i<width;i++){ //uv.y-width*texelSize.y;i<uv.y+width*texelSize.y;i+=texelSize.y){
            total += texture(tex, vec2(uv.x + i*texelSize.x, uv.y)).xyz * weight[i];
            total += texture(tex, vec2(uv.x - i*texelSize.x, uv.y)).xyz * weight[i];
        }
    }
    float max_depth = 1;
    if (texture(tex, uv).x==max_depth){
        //discard;
    }
    fragColor = vec4(total.xy, texture(tex, uv).za);


    // TODO: [Task 10] Implement vertical blurring
}
