#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform vec2 iResolution;
uniform float iTime;
vec2 fragCoord = gl_FragCoord.xy;
void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - 1.0;//transform from [0,1] to [-1,1]

    vec3 res = vec3(uv, 0.0);

    res = sin(res * 10.0 + iTime) * 0.5 + 0.5;

    FragColor = vec4(res, 1.0);
}