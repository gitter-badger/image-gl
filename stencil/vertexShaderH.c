attribute highp vec4 aVertexPosition;                         
attribute lowp vec4 aColor;                                   
varying lowp vec4 vColor;                                     
uniform highp mat4 uMVMatrix;                                 
uniform highp mat4 uPMatrix;                                  
void main() {                                                 
   vColor = aColor;                                           
   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;      
};                                                            
