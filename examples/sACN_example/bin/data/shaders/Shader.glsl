//https://www.shadertoy.com/view/Mdcczr

#ifdef GL_ES
precision highp float;
#endif

#define     WAVE .75
#define SPECTRUM .25

#define      PI 3.141592653589793
#define     TAU 6.283185307179586
#define HALF_PI 1.570796326794896

#define FUCHSIA vec3(.882, .192, .659)
#define RHUBARB vec3(.772, .125, .227)
#define CARMINE vec3(.831, .145, .369)

float norm(float t, float a, float b) {
    return (t - a) / (b - a);
}

float map(float t, float a, float b, float c, float d) {
    return norm(t, a, b) * (d - c) + c;
}

vec4 blorb(vec3 col, vec2 pq, sampler2D channel) {
    float audio = texture(channel, vec2(pq.x, SPECTRUM)).x;
    float dilation = .25;
    float blur = pow(pq.x * 2., 7.);
    float x = audio - pq.y + dilation;
    float mask = smoothstep(0., blur, x);
    mask = sin(iTime * .15) + 1. + mask;
    return vec4(col, 1.) * mask;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = ( 2.*fragCoord - iResolution.xy ) / iResolution.y*2;
    vec2 pq = vec2(atan(uv.x * -sin(iTime) + .5, uv.y * cos(iTime) + .5), length(uv));
    pq.x = pq.x / TAU + .5;
    
    vec2 p0 = pq;
    p0.x = map(p0.x * (sin(iTime * .5) + .5), 0., 1., .1, .35);
    
    vec2 p1 = pq;
    p1.x = map(p1.x, 0., 1., .12, .38);
    
    vec2 p2 = pq;
    p2.x = map(p2.x, 0., 1., .13, .41);
    
    vec4 b0 = blorb(FUCHSIA, p0, iChannel0);
    vec4 b1 = blorb(RHUBARB, p1, iChannel0);
    vec4 b2 = blorb(CARMINE, p2, iChannel0);
    
    vec4 c = mix(b2, b1, b1.a * .5);
    c = mix(c, b0, b0.a * (sin(iTime * .5) + .5));
    
    fragColor = c;
}
