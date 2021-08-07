//https://www.shadertoy.com/view/Mdcczr

#ifdef GL_ES
precision highp float;
#endif
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy - .5 -iTime*.005;
    
    float freqs[4];
    //Sound
    freqs[0] = texture( iChannel0, vec2( 0.01, 0.25 ) ).x;
    freqs[1] = texture( iChannel0, vec2( 0.07, 0.25 ) ).x;
    freqs[2] = texture( iChannel0, vec2( 0.15, 0.25 ) ).x;
    freqs[3] = texture( iChannel0, vec2( 0.30, 0.25 ) ).x;
    vec3 au = vec3(freqs[0], freqs[1], freqs[2]);
    vec3 c = cos(vec3(iTime*.006*au.r*0.005,
                      iTime*.025*au.g*0.0005,
                      iTime*.0095*au.b*0.00005)*3.)*2.+2.;
    for (int i = 0; i < 27; i++) {
        vec3 p = vec3(uv*float(i),float(i));
        c += vec3( sin(c.y+sin(p.x)),
                  cos(c.z+sin(p.z)),
                  -sin(c.x+sin(p.y)) );
        
    }
    
    c.r*=au.r*2.;
    c.g*=au.g*2.;
    c.b*=au.b*2.;
    
    fragColor = vec4(c*c*.004,1.);
}
