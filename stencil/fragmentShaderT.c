varying vec2 vTextureCoord;                                                                         
uniform vec4 uColor;                                                                                
uniform sampler2D uSampler;                                                                         
void main() {                                                                                       
   gl_FragColor = vec4( 1,1,1, texture2D( uSampler, vTextureCoord ).a) * uColor;                    
};                                                                                                  
