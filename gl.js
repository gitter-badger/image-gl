var gl;
    
function start() {
        var canvas = document.getElementById("glcanvas");

        gl = initWebGL(canvas);

        if(gl) {
            gl.clearColor(0.0,0.0,0.0,1.0);
            gl.enable(gl.DEPTH_TEST);
            gl.depthFunc(gl.LEQUAL);
            gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

            initTextures();
         }else{
            alert("GL is not init!");
         }
    }

function initWebGL(canvas){
         gl = null;

         try {
            gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
         }
         catch(e){
        }

         if(!gl){
            alert ("Unable to init WeGL");
            gl = null;
         }

         return gl;
}

function initTextures() {
        cubeTexture = gl.createTexture();
        cubeImage = new Image();
        cubeImage.onload = function() {
             handleTextureLoaded(cubeImage, cubeTexture);
        }
        cubeImage.src = "2.png";
}

function handleTextureLoaded(image, texture){
         try{
            gl.bindTexture(gl.TEXTURE_2D, texture);
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
            gl.generateMipmap(gl.TEXTURE_2D);
            gl.bindTexture(gl.TEXTURE_2D, null);
         }catch(e){
             alert(e);
         }
}
