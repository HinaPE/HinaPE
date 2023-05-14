#version 430
in vec2 gPosition;
uniform mat4 proj;
uniform float particleRadius;
flat in int vert_num;
void main() {
    vec3 n = vec3(2.0 * gPosition, 0.0);
    float r2 = dot(n.xy, n.xy);
    if (r2 > 1.0) discard;
    n.z = sqrt(1.0 - r2);
    vec3 L = normalize(vec3(1.0));
    float d = max(0.0, dot(L, n));
    //out_color = vec4(d * color.xyz, color.w);
    float near = .1;
    float far = 4.0;
    float world_space_dist = normalize(n).z*particleRadius;
    float delta = (world_space_dist-near)/(far-near);
    //gl_FragDepth=gl_FragCoord.z;



    float A = proj[2][2];
    float B = proj[3][2];
    float C = (A-1.f)/2.f;
    float D = B/2.0;
    float camz = ((gl_FragCoord.z/gl_FragCoord.w)-D)/C;
    camz += world_space_dist/200.0;

    gl_FragDepth = ((camz*C)+D)*gl_FragCoord.w;
}


/*
vec3 uvToEye(vec2 texCoord) {
    float depth = texture(tex,texCoord).x;
    vec2 transUV = (texCoord - vec2(.5))*2;
    vec4 sscoord = vec4(transUV,depth*2.0 - 1.0,1);
    mat4 invproj = inverse(proj);
    vec4 eyepos = invproj * sscoord;
    return eyepos.xyz/eyepos.w;
}
*/

