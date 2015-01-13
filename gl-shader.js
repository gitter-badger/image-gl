/*
 * This file contains methods to set up the shaders for the scene
 */

var shaderProgram;

var uBCG = new Array(4);
var aBCG = new Array(4);
var zBCG = Create2DArray(256, 4);

function updateBCG( brightness, contrast, gamma ) {
	gl.uniform4f(shaderProgram.uBCG, brightness, contrast, gamma , 1.0);
}

function getShader(gl, id) {
	var shaderScript = document.getElementById(id);
	if (!shaderScript) {
		return null;
	}

	var str = "";
	var k = shaderScript.firstChild;

	while (k) {
		if (k.nodeType == 3) {
			str += k.textContent;
		}
		k = k.nextSibling;
	}

	var shader;
	if (shaderScript.type == "x-shader/x-fragment") {
		shader = gl.createShader(gl.FRAGMENT_SHADER);
	} else if (shaderScript.type == "x-shader/x-vertex") {
		shader = gl.createShader(gl.VERTEX_SHADER);
	} else {
		return null;
	}

	gl.shaderSource(shader, str);
	gl.compileShader(shader);

	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		alert(gl.getShaderInfoLog(shader));
		return null;
	}

	return shader;
}

function initShaders() {
	var fragmentShader = getShader(gl, "shader-fs");
	var vertexShader = getShader(gl, "shader-vs");

	shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		alert("Could not initialise shaders");
	}

	gl.useProgram(shaderProgram);

	shaderProgram.vertexPositionAttribute = gl.getAttribLocation( shaderProgram, "aVertexPosition" );
	gl.enableVertexAttribArray( shaderProgram.vertexPositionAttribute );

	shaderProgram.textureCoordAttribute   = gl.getAttribLocation( shaderProgram, "aTextureCoord" );
	gl.enableVertexAttribArray( shaderProgram.textureCoordAttribute );
	
	shaderProgram.uBCG = gl.getUniformLocation( shaderProgram, "uBCG" );
	shaderProgram.zBCG = gl.getUniformLocation( shaderProgram, "zBCG" );
	
	shaderProgram.pMatrixUniform  = gl.getUniformLocation( shaderProgram, "uPMatrix" );
	shaderProgram.mvMatrixUniform = gl.getUniformLocation( shaderProgram, "uMVMatrix" );
	shaderProgram.samplerUniform = gl.getUniformLocation( shaderProgram, "uSampler" );
}