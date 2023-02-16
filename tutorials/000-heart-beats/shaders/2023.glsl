#version 330 core
out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;
vec2 fragCoord = gl_FragCoord.xy;

// segment function modified from https://www.shadertoy.com/view/MdfGzf
float segment(vec2 uv, bool On)
{
    if (!On) return 0.0;
    float seg = (1.0-smoothstep(0.08,0.09+float(On)*0.02,abs(uv.x)))*
    (1.0-smoothstep(0.46,0.47+float(On)*0.02,abs(uv.y)+abs(uv.x)));
    if (On) seg *= (1.0-length(uv*vec2(3.8,0.9)));//-sin(iTime*25.0*6.26)*0.04;
    else seg *= -(0.05+length(uv*vec2(0.2,0.1)));
    return seg;
}

// sevenSegment function modified from https://www.shadertoy.com/view/MdfGzf
float sevenSegment(vec2 uv,int num)
{
    float seg= 0.0;
    seg += segment(uv.yx+vec2(-1.0, 0.0),num!=-1 && num!=1 && num!=4                    );
    seg += segment(uv.xy+vec2(-0.5,-0.5),num!=-1 && num!=1 && num!=2 && num!=3 && num!=7);
    seg += segment(uv.xy+vec2( 0.5,-0.5),num!=-1 && num!=5 && num!=6                    );
    seg += segment(uv.yx+vec2( 0.0, 0.0),num!=-1 && num!=0 && num!=1 && num!=7          );
    seg += segment(uv.xy+vec2(-0.5, 0.5),num==0 || num==2 || num==6 || num==8           );
    seg += segment(uv.xy+vec2( 0.5, 0.5),num!=-1 && num!=2                              );
    seg += segment(uv.yx+vec2( 1.0, 0.0),num!=-1 && num!=1 && num!=4 && num!=7          );

    return seg;
}

// showNum function modified from https://www.shadertoy.com/view/MdfGzf
float showNum(vec2 uv,int nr)
{
    if (abs(uv.x)>1.5 || abs(uv.y)>1.2) return 0.0;
    float seg= 0.0;
    if (uv.x>0.0){
        nr /= 10;
        seg += sevenSegment(uv+vec2(-0.75,0.0),nr);
    } else seg += sevenSegment(uv+vec2( 0.75,0.0),int(mod(float(nr),10.0)));
    return seg;
}

// hash function copy from https://www.shadertoy.com/view/4djSRW
float hash12(vec2 p)
{
    vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 colorPalette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(2.*3.1415926*(c*t+d));
}

float noise(vec2 pos) {
    vec2 i = floor(pos);
    vec2 f = fract(pos);

    float a = hash12(i);
    float b = hash12(i + vec2(1, 0));
    float c = hash12(i + vec2(0, 1));
    float d = hash12(i + vec2(1, 1));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

mat2 rotate2d(float angle){
    return mat2(cos(angle),-sin(angle),
    sin(angle),cos(angle));
}

void main() {
    vec2 uv = (fragCoord.xy-0.5*iResolution.xy) / iResolution.y;
    vec3 color0 = 0.1 + 0.2*cos(iTime*2.+uv.xyx+vec3(0,2,4));

    uv *= vec2(-4, 4);
    uv.x += uv.y/12.0*sin(iTime*2.);
    uv = rotate2d(noise((uv + vec2(iTime*.5)) * 2.5 + .5) * .2) * uv;

    uv.x += 1.5;
    float seg = 0.0;
    seg += showNum(uv,23);
    uv.x -= 3.;
    seg += showNum(uv,20);

    vec3 color1 = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

    vec3 finalColor = vec3(0);
    finalColor = colorPalette(uv.x*.1 + iTime*.5, vec3(.5), vec3(.5), vec3(1.), vec3(0, .33, .67));
    finalColor *= seg;
    finalColor = smoothstep(color1, finalColor, 1. - vec3(seg));
    finalColor += smoothstep(0.8, 0.9, 1. - seg) * color0;

    FragColor = vec4(finalColor,1.0);
}
