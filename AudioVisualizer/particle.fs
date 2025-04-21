#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;
uniform float magnitudes[64];

#define PI 3.14159

void main(){
    vec2 p = (gl_FragCoord.xy - 0.5 * resolution) / min(resolution.x, resolution.y);
    vec2 t = vec2(gl_FragCoord.xy / resolution);
    
    vec3 c = vec3(0.0);

    for(int i = 0; i < 20; i++) {
        float ti = 0.4 * PI * float(i) / 30.0 * time * 5.5;
        float x = cos(3.0 * ti);
        float y = sin(4.0 * ti);
        vec2 o = 0.40 * vec2(x, y);

        float dist = length(p - o);
        float intensity = 0.01 / dist;

        // Use magnitude to modulate brightness and hue
        int band = int(float(i) / 20.0 * 64.0);  // map i to magnitudes[0..63]
        float mag = magnitudes[band] * .5;
        float r = fract(x + mag);
        float g = 0.9 + mag * 0.8;
        float b = 1.0 - r + mag * 0.5;

        //1.0 - r + mag * 0.5;

        c += intensity * vec3(r, g, b);
    }

    gl_FragColor = vec4(c, 1.0);
}
