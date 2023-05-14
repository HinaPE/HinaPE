#version 330
uniform sampler2D tex;
uniform sampler2D thickness;
uniform sampler2D scene_color;
uniform sampler2D scene_depth;
in vec2 uv;
out vec4 fragColor;
uniform mat4 proj;
uniform mat4 view;

//settings
uniform bool doNoise;
uniform bool doRefracted;
uniform bool doReflected;
uniform bool doSpecular;
uniform bool doRenderNormals;

vec3 uvToEye(vec2 texCoord) {
    float depth = texture(tex, texCoord).x;
    vec2 transUV = (texCoord - vec2(.5))*2;
    vec4 sscoord = vec4(transUV, depth*2.0 - 1.0, 1);
    mat4 invproj = inverse(proj);
    vec4 eyepos = invproj * sscoord;
    return eyepos.xyz/eyepos.w;
}
void main() {
    vec2 texelSize = 1.0 / textureSize(tex, 0).xy;
    float z = texture(tex, uv).x;
    float thick = texture(thickness, uv).x;
    float noise = texture(thickness, uv).z;
    vec4 sceneColor = texture(scene_color, uv);
    float sceneDepth = texture(scene_depth, uv).x;
    if (z>=(sceneDepth) || z==1){
        fragColor = vec4(sceneColor.rgb, 1);
        return;
    }
    vec3 eyepos = uvToEye(uv);
    vec3 ddx = uvToEye(uv+vec2(texelSize.x, 0)) - eyepos;
    vec3 ddx2 = eyepos - uvToEye(uv-vec2(texelSize.x, 0));
    if (abs(ddx.z)>abs(ddx2.z)){
        ddx = ddx2;
    }

    vec3 ddy = uvToEye(uv+vec2(0, texelSize.y))-eyepos;
    vec3 ddy2 = eyepos - uvToEye(uv-vec2(0, texelSize.y));
    if (abs(ddy.z)>abs(ddy2.z)){
        ddy = ddy2;
    }


    vec3 normal = normalize(cross(ddx, ddy));

    vec4 light_pos_view_space = view * vec4(-2, 2, -2, 1);

    vec3 water_color = vec3(.3, .5, .7);

    if (doNoise){
        vec2 texelSize = 1.0 / textureSize(tex, 0).xy;

        float z = texture(thickness, uv).z;
        float zxp = texture(thickness, uv+vec2(texelSize.x, 0)).z;
        float zxn = texture(thickness, uv-vec2(texelSize.x, 0)).z;
        float zyp = texture(thickness, uv+vec2(0, texelSize.y)).z;
        float zyn = texture(thickness, uv-vec2(0, texelSize.y)).z;

        texelSize = vec2(1.f);
        float dzdx = (zxp-zxn)/(2.0 * texelSize.x);//maybe not times texelSize
        float dzdy = (zyp-zyn)/(2.0 * texelSize.y);//maybe not times texelSize
        vec3 perp_amount = vec3(dzdx, dzdy, 0);
        perp_amount = mix(clamp((1-normal.z), .05, .5)*perp_amount, vec3(0), exp(-thick*.8));//blend to make single drops look better
        normal = normalize(normal + perp_amount);
    }
    //fresnel
    vec3 n = normal;
    vec3 eyeToVertex = normalize(eyepos);
    vec3 E = vec3(0, 0, 1);
    vec3 vertexToEye = -eyeToVertex;
    float r0 = 0.2;
    float F = r0 + (1.f-r0)*pow((1-dot(vertexToEye, n)), 5);

    //refracted
    float B = thick*.0035;
    vec2 refracted_sample = uv + (B*n.xy);
    vec3 refracted_scene_color = texture(scene_color, refracted_sample).rgb;
    vec3 alpha = mix(water_color, refracted_scene_color, exp(-thick*.1));
    vec3 refracted = clamp(alpha*(1-F), 0, 1);

    //reflected
    vec3 beta = vec3(.529, .808, .922);//should use cubemap
    //beta = mix(beta,sceneColor.rgb,exp(-thick));//should use cubemap also this is hacked
    vec3 reflected = clamp(beta*F, 0, 1);

    //specular
    vec3 l = normalize(light_pos_view_space.xyz - eyepos);
    vec3 H = normalize(l+vertexToEye);
    vec3 specular_color = clamp(vec3(1)* pow(clamp(dot(n, H), 0, 1), 80), 0, 1);

    vec3 final_color = vec3(0);
    if (doRefracted){
        final_color += refracted;
    }
    if (doReflected){
        final_color += reflected;
    }
    if (doSpecular){
        final_color += specular_color;
    }

    fragColor = vec4(final_color, 1);

    if (doRenderNormals){
        fragColor = vec4((normal + 1.0)/(2.0), 1);
    }
}
