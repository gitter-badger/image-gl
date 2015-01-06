var gl;
    
function start() {
        var canvas = document.getElementById("glcanvas");

        gl = initWebGL(canvas);

        if(gl) {
            gl.clearColor(0.0,0.0,0.0,1.0);
            gl.enable(gl.DEPTH_TEST);
            gl.depthFunc(gl.LEQUAL);
            gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

         }else{
            alert("GL is not init!");
         }
    }

function initWebGL(canvas){
         gl = null;

         try {
         	// Try to grab the standard context. If it fails, fallback to experimental.
            gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
         }
         catch(e){
        }

		// If we don't have a GL context, give up now	
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

function initShaders() {
	var fragmentShader = getShader(gl, "shader-fs");
	var vertexShader = getShader(gl, "shader-vs");
	
	// Create Shader Program
	
	try{
	shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);
	
	// If creating the shader program failed, alert
	
	if(!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		alert("Unable to initialize the shader program.");
	}
	
	gl.useProgram(shaderProgram);
	
	vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
	gl.enableVertexAttribArray(vertexPostitionAttribute);
	}catch(e){
		alert(e);
	}
}

function getShader(gl, id) {
	var shaderScript, theSource, currentChild, shader;
	
	shaderScript = document.getELementById(id);
	
	if(!shaderScript) {
		return null;
	}
	
	theSource = "";
	currentChild = shaderScript.firstChild;
	
	while(currentChild) {
		if (currentChild.nodeType == currentChild.TEXT_NODE) {
			theSource += currentChild.textContent;
		}	
		currentChild = currentChild.nextSibling;
	}
	
	if(shaderScript.text == "x-shader/x-fragment") {
		shader = gl.createShader(gl.FRAGMENT_SHADER);
	}else if(shaderScript.type == "x-shader/x-vertex"){
		shader = gl.createShader(gl.VERTEX_SHADER);
	} else {
		// unknown shader type
		return null;
	}
	gl.shaderSource(shader, theSource);
	
	// Compile the shader program
	gl.compileShader(shader);
	
	// See if it compiled
	if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		alert("An error occurred compiling shaders: " + gl.getShaderInfoLog(shader));
		return null;
	}	
	
	return shader;
}

var horizAspect = 480.0/640.0;

function initBuffers() {
	squareVerticesBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVerticesBuffer);
	
	var vertices = [
		 1.0,  1.0,  0.0,
		-1.0,  1.0,  0.0,
		 1.0, -1.0,  0.0,
		-1.0, -1.0,  0.0
	];
	
	gl.bufferedData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
}

function drawScene() {
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	perspectiveMatrix = makePerspective(45, 640.0/480.0, 0.1, 100.0);
	
	loadIdentity();
	mvTranslate([-0.0, 0.0, -6.0]);
	
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVerticesBuffer);
	gl.vertexAttribPointer(vertexPositionAttribute, 3, gl.FLOAT, false, 0, 0);
	setMatrixUniforms();
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
}

function loadIdentity() {
	mvMatrix = Matrix.I(4);
}

function multMatrix(m) {
	mvMatrix = mvMatrix.x(m);
}

function mvTranslate(v) {
	multMatrix(Matrix.Translation($V([v[0], v[1], v[2]])).ensure4x4());
}

function setMatrixUniforms() {
	var pUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	gl.uinformMatrix4fv(pUniform, false, new Float32Array(perspectiveMatrix.flatten()));
	
	var mvUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
	gl.uniformMatrix4fv(mvUniform, false, new Float32Array(mvMatrix.flatten()));
}