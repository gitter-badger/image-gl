/*
 * This file contains methods to set up the shaders for the scene
 */

var shaderProgramG;
var shaderProgramS;


var uInvert = 0;
var uBCG = new Array(4);

function updateBCG( brightness, contrast, gamma ) {
	gl.uniform4f(shaderProgramG.uBCG, brightness, contrast, gamma , 1.0);
}

function updateInvert( invert ){
	if(invert){
		gl.uniform1i(shaderProgramG.uInvert, 1);
	}else{
		gl.uniform1i(shaderProgramG.uInvert, 0);
	}
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
    {
        var vertexShader = getShader(gl, "shader-vs-s");
        var fragmentShader = getShader(gl, "shader-fs-s");

        shaderProgramS = gl.createProgram();
        gl.attachShader(shaderProgramS, vertexShader);
        gl.attachShader(shaderProgramS, fragmentShader);
        gl.linkProgram(shaderProgramS);

        if (!gl.getProgramParameter(shaderProgramS, gl.LINK_STATUS)) {
            var lastError = gl.getProgramInfoLog(shaderProgramS);
            alert("Error in program linking:" + lastError);
            gl.deleteProgram(shaderProgramS);
        } else {
            gl.useProgram(shaderProgramS);

            shaderProgramS.vertexPositionAttribute = gl.getAttribLocation(shaderProgramS, "aVertexPosition");

            shaderProgramS.pMatrixUniform = gl.getUniformLocation(shaderProgramS, "uPMatrix");
            shaderProgramS.mvMatrixUniform = gl.getUniformLocation(shaderProgramS, "uMVMatrix");
        }
    }

    {
        var fragmentShader = getShader(gl, "shader-fs-g");
        var vertexShader = getShader(gl, "shader-vs-g");

        shaderProgramG = gl.createProgram();
        gl.attachShader(shaderProgramG, vertexShader);
        gl.attachShader(shaderProgramG, fragmentShader);
        gl.linkProgram(shaderProgramG);

        if (!gl.getProgramParameter(shaderProgramG, gl.LINK_STATUS)) {
            alert("Could not initialise shaders");
        }

        gl.useProgram(shaderProgramG);

        shaderProgramG.vertexPositionAttribute = gl.getAttribLocation(shaderProgramG, "aVertexPosition");
        shaderProgramG.textureCoordAttribute   = gl.getAttribLocation(shaderProgramG, "aTextureCoord");

        shaderProgramG.uBCG                    = gl.getUniformLocation(shaderProgramG, "uBCG");
        shaderProgramG.uInvert                 = gl.getUniformLocation(shaderProgramG, "uInvert");

        shaderProgramG.pMatrixUniform          = gl.getUniformLocation(shaderProgramG, "uPMatrix");
        shaderProgramG.mvMatrixUniform         = gl.getUniformLocation(shaderProgramG, "uMVMatrix");
        shaderProgramG.samplerUniform          = gl.getUniformLocation(shaderProgramG, "uSampler");

        shaderProgramG.colorUniform            = gl.getUniformLocation(shaderProgramG, "uColor");
    }
}