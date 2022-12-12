#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
vec2 fragCoord = gl_FragCoord.xy;

uniform vec2 iResolution;
void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - 1.0;//transform from [0,1] to [-1,1]

    vec3 res;

    res.x = sin(uv.x * 10.0) * 0.5 + 0.5;
    res.y = sin(uv.y * 10.0) * 0.5 + 0.5;
    res.z = sin((uv.x + uv.y)  * 10.0) * 0.5 + 0.5;

    FragColor = vec4(res, 1.0);
}