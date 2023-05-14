#version 420
uniform sampler2D tex;
uniform vec2 screen_size;
uniform mat4 p;
in vec2 uv;

float meanCurvature(){
    vec2 texelSize = 1.0 / textureSize(tex,0).xy;
    float Fx = -p[0][0];
    float Fy = -p[1][1];;
    float Vx = screen_size.x;
    float Vy = screen_size.y;
    float Cx = 2.0/(Vx*Fx);
    float Cy = 2.0/(Vx*Fx);
    float z = texture(tex,uv).x;
    float zxp = texture(tex,uv+vec2(texelSize.x,0)).x;
    float zxn = texture(tex,uv-vec2(texelSize.x,0)).x;
    float zpxpy = texture(tex,uv+texelSize).x;
    float znxpy = texture(tex,uv+(texelSize*vec2(-1,1))).x;
    float zpxny = texture(tex,uv+(texelSize*vec2(1,-1))).x;
    float znxny = texture(tex,uv+(texelSize*vec2(-1,-1))).x;
    float zyp = texture(tex,uv+vec2(0,texelSize.y)).x;
    float zyn = texture(tex,uv-vec2(0,texelSize.y)).x;

    //trying to figure out boundary conditions
    //if(gl_FragCoord.x>screen_size.x/2){
        float depth_diff = .00005;//need to play around with this
    if(zxp==1 || abs(zxp-z)>depth_diff){
        return 0;
    }
    if(zxn==1 || abs(zxn-z)>depth_diff){
        return 0;
    }
    if(zyp==1 || abs(zyp-z)>depth_diff){
        return 0;
    }
    if(zyn==1 || abs(zyn-z)>depth_diff){
        return 0;
    }
   // }

    texelSize = vec2(1.f);
    float dzdx = (zxp-zxn)/(2.0 * texelSize.x);//maybe not times texelSize


    float dzdy = (zyp-zyn)/(2.0 * texelSize.y);//maybe not times texelSize

    float dzd2x = (zxp + zxn - 2.0*z)/(texelSize.x*texelSize.x);//maybe not ivide
    float dzd2y = (zyp + zyn - 2.0*z)/(texelSize.y*texelSize.y); //maybe not divide



    float dzdxy = (((zpxpy - znxpy) / (2.0*texelSize.x)) -
                    ((zpxny - znxny) / (2.0*texelSize.x))) / (2.0*texelSize.y);//  - zpxny + znxny - znxpy) / (4.0 *)

    float dDdx = 2.0*Cy*Cy*dzdx*dzd2x + 2.0*Cx*Cx*dzdy*dzdxy + 2.0*Cx*Cx*Cy*Cy*z*dzdx;
    float dDdy = 2.0*Cy*Cy*dzdx*dzdxy + 2.0*Cx*Cx*dzdy*dzd2y + 2.0*Cx*Cx*Cy*Cy*z*dzdy;

    float D = Cy*Cy*dzdx*dzdx+Cx*Cx*dzdy*dzdy+Cx*Cx*Cy*Cy*z*z;



    float Ex = .5 * dzdx * dDdx - (dzd2x*D);
    float Ey = .5 * dzdy * dDdy - (dzd2y*D);

    float twoH = (Cy*Ex + Cx*Ey)/(pow(D,1.5));

    return twoH*.5;


}

void main(void)
{
    if(texture(tex,uv).x>=1){
        discard;
    }
    float H = meanCurvature();
    gl_FragDepth = texture(tex,uv).x + H * 0.00025f;
}
