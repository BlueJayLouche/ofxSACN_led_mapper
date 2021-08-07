// Author: paperu
// Title: quantum field

float t;
#define P 6.283185307

vec3 SpectrumPoly(in float x) {
    // https://www.shadertoy.com/view/wlSBzD
    return (vec3( 1.220023e0,-1.933277e0, 1.623776e0)+(vec3(-2.965000e1, 6.806567e1,-3.606269e1)+(vec3( 5.451365e2,-7.921759e2, 6.966892e2)+(vec3(-4.121053e3, 4.432167e3,-4.463157e3)+(vec3( 1.501655e4,-1.264621e4, 1.375260e4)+(vec3(-2.904744e4, 1.969591e4,-2.330431e4)+(vec3( 3.068214e4,-1.698411e4, 2.229810e4)+(vec3(-1.675434e4, 7.594470e3,-1.131826e4)+ vec3( 3.707437e3,-1.366175e3, 2.372779e3)*x)*x)*x)*x)*x)*x)*x)*x)*x;
}
mat2 rot(in float a) { return mat2(cos(a),sin(a),-sin(a),cos(a)); }

float box(in vec3 p, in float s) { p = abs(p) - s; return max(p.x,max(p.y,p.z)); }
float cyl(in vec3 p, in float h) { return length(p.xz) - h; }

float df(in vec3 p) {
    vec3 pp = p;
    
    float d = 10e9;
    
    for(int i = 0; i < 15; i++) {
        d = min(d, cyl(p, -.002));
        p.xy *= rot(P/3. + t + length(p)*2.);
        p.z = abs(p.z) - .5;
        p.xz *= rot(P/3. - t + length(p)*5.1);
        p.x = abs(p.x) - .5;
    }
    
    return max(d, box(pp, .75));
}

#define LIM .001
#define MAX_D 5.
#define MAX_IT 50
int rm(in vec3 c, in vec3 r) {
    vec3 p = c;
    int it = 0;
    bool hit = false;
    for(int i = 0; i < MAX_IT; i++) {
        float d = df(p);
        if(d < LIM || distance(c,p) > MAX_D) break;
        p += d*r;
        it = i;
    }
    return it;
}

vec3 plane2sphere(in vec2 p) {
    float t = -4./(dot(p,p) + 4.);
    return vec3(-p*t, 1. + 2.*t);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 st = (fragCoord.xy - iResolution.xy*.5)/iResolution.y;
    t = iTime*.25 + 3.;
    
    vec3 c = vec3(0.,0.,0.);
    vec3 r = normalize(vec3(st,1.));
    r = plane2sphere(st*12.);
    
    r.xy *= rot(t);
    r.xz *= rot(t);
    r.yz *= rot(t);
    
    int it = rm(c,r);
    
    float s = pow(float(it)*.02,2.);
    vec3 color = mix(vec3(0.), SpectrumPoly((max(1.-s,.55))), s*1.);
    color = mix(color*1., vec3(1.), s);
    
    fragColor = vec4(color,1.0);
}
