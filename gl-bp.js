var gl;
var m_useMipMap = false;
var m_antiAliasingEnabled = false;
var pi = 3.14159265359;

var shaderProgram;

var mvMatrix = mat4.create();
var mvMatrixStack = [];
var pMatrix = mat4.create();

/// TODO: Use texture management from here:
// http://blog.tojicode.com/2012/03/javascript-memory-optimization-and.html

function mvPushMatrix() {
    var copy = mat4.create();
    mat4.set(mvMatrix, copy);
    mvMatrixStack.push(copy);
}

function mvPopMatrix() {
    if (mvMatrixStack.length == 0) {
		throw "Invalid popMatrix!";
    }
	mvMatrix = mvMatrixStack.pop();
}

function setMatrixUniforms() {
	gl.uniformMatrix4fv(shaderProgramG.pMatrixUniform, false, pMatrix);
	gl.uniformMatrix4fv(shaderProgramG.mvMatrixUniform, false, mvMatrix);
}

function setMatrixUniformsS() {
    gl.uniformMatrix4fv(shaderProgramS.pMatrixUniform, false, pMatrix);
    gl.uniformMatrix4fv(shaderProgramS.mvMatrixUniform, false, mvMatrix);
}

//Util: GL Exceptions
function throwOnGLError(err, funcName, args) {
	throw WebGLDebugUtils.glEnumToString(err) + " was caused by call to: " + funcName;
}

//Util: GL Logging
function logGLCall(functionName, args) {
	//console.log("gl." + functionName + "(" +
	//	WebGLDebugUtils.glFunctionArgsToString(functionName, args) + ")");
}

//Util: Check for undefined
function validateNoneOfTheArgsAreUndefined(functionName, args) {
  for (var ii = 0; ii < args.length; ++ii) {
    if (args[ii] === undefined) {
      console.error("undefined passed to gl." + functionName + "(" +
                     WebGLDebugUtils.glFunctionArgsToString(functionName, args) + ")");
    }
  }
}

//Util: GL 
function logAndValidate(functionName, args) {
   logGLCall(functionName, args);
   validateNoneOfTheArgsAreUndefined (functionName, args);
}

function initGL(canvas) {
	try {
		gl = canvas.getContext("experimental-webgl",
				{antialias:m_antiAliasingEnabled});
		if(gl == null){
			gl = canvas.getContext("webgl",
				{antialias:m_antiAliasingEnabled});
		}

		gl = WebGLDebugUtils.makeDebugContext(gl, throwOnGLError, logAndValidate);

		gl.viewportWidth = canvas.width;
		gl.viewportHeight = canvas.height;

		canvas.addEventListener('webglcontextlost', function(){
		    // context is lost
		}, false);

		canvas.addEventListener('webglcontextrestored', function(){
		    // context is restored
		}, false);

		canvas.addEventListener('mousewheel', mouseWheelHandler, false);
		canvas.addEventListener('DOMMouseScroll', mouseWheelHandler, false);
		canvas.addEventListener('contextmenu', function(ev) {
    		ev.preventDefault();
    		return false;
		}, false);

	} catch (e) {
		alert(e);
	}
    if (!gl) {
		alert("Could not initialise WebGL, sorry :-(");
    }
}

function tick() {
	requestAnimFrame(tick);
	handleKeys();
	drawScene();
	animate();
}

window.setTimeout(tick, 1000);

function webGLStart() {
	var canvas = document.getElementById("gl-canvas");
	
	canvas.width = document.body.clientWidth;
	canvas.height = document.body.clientHeight;
	
	initGL(canvas);

	initShaders();
	
	initBuffersAndTextures();

	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	gl.enable(gl.DEPTH_TEST);

    canvas.onmousedown = handleMouseDown;
    document.onmouseup = handleMouseUp;
    document.onmousemove = handleMouseMove;

	document.onkeydown = handleKeyDown;
	document.onkeyup = handleKeyUp;

	tick();
}

function handleLoadedTexture(texture) {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

    if(m_useMipMap){
    	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
    	gl.generateMipmap(gl.TEXTURE_2D);
    }else{
    	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    }
    //UNUSED: Used for clamping textures that are not power of 2
    gl.texParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	gl.texParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.bindTexture(gl.TEXTURE_2D, null);
}


