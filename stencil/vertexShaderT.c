uniform mat4 uMVMatrix;                                                    
uniform mat4 uPMatrix;                                                     
attribute vec4 aVertexPosition;                                            
varying vec2 vTextureCoord;                                                
void main(void) {                                                          
   gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition.xy, 0, 1);    
   vTextureCoord = aVertexPosition.zw;                                     
};                                                                         
