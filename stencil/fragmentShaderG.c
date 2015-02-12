precision mediump float;
varying vec2 vTextureCoord;
uniform int uInvert;
uniform sampler2D uSampler;
uniform vec4 uBCG;
void main(void) {
   vec4 vColor;
   mediump float brightness = uBCG.x;
   mediump float contrast = uBCG.y;
   mediump float gamma = uBCG.z;
    
    mediump float b = brightness / 100.0;
    mediump float c = contrast / 100.0;
    mediump float g;
    if (gamma > 50.0) {
    g = 1.0 + (gamma - 50.0) / 10.0;
    } else {
    g = 1.0 / (1.0 + (50.0 - gamma) / 10.0);
    }
    mediump float bias = (1.0 - c) / 2.0 + b * c;
                              
    vColor.x = (pow(((vColor.x * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;
    vColor.y = (pow(((vColor.y * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;
    vColor.z = (pow(((vColor.z * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;
    if(uInvert > 0){
    vColor.x = 1.0 - vColor.x;
    vColor.y = 1.0 - vColor.y;
    vColor.z = 1.0 - vColor.z;
    }
                                                
    if(vColor.a == 0.0){
    vColor.x = 0.0;
    vColor.y = 0.0;
    vColor.z = 0.0;
    vColor.a = 1.0;
    }
    gl_FragColor = vColor;
}
