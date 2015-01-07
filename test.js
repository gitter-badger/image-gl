var gl;
var m_maxZ = -1;
var m_minZ = -20;
var m_initZ = -7;
var m_antiAliasingEnabled = false;

// Original dimensions
var oriW = 800;
var oriH = 400;

// Texture dimensions
var texW = 1024;
var texH = 1024;

var zoom = m_initZ;
var transX = 0.0;
var transY = 0.0;
var rotation = 0.0;

var shaderProgram;

var squareVertexPositionBuffer;

var mvMatrix = mat4.create();
var pMatrix = mat4.create();

function logGLCall(functionName, args) {   
   console.log("gl." + functionName + "(" + 
      WebGLDebugUtils.glFunctionArgsToString(functionName, args) + ")");   
} 

function initGL(canvas) {
	try {
		gl = canvas.getContext("experimental-webgl", 
				{antialias:m_antiAliasingEnabled});
		if(gl == null){
			gl = canvas.getContext("webgl", 
				{antialias:m_antiAliasingEnabled});
		}
		
		gl = WebGLDebugUtils.makeDebugContext(gl, undefined, logGLCall);
		
		gl.viewportWidth = canvas.width;
		gl.viewportHeight = canvas.height;
		
		canvas.addEventListener('webglcontextlost', function(){
		    // context is lost
		}, false);
		
		canvas.addEventListener('webglcontextrestored', function(){
		    // context is restored
		}, false);

	} catch (e) {
		alert(e);
	}
    if (!gl) {
		alert("Could not initialise WebGL, sorry :-(");
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

	shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
	gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

	shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
}



function setMatrixUniforms() {
	gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
	gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}




function initBuffers() {
        triangleVertexPositionBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexPositionBuffer);
        var vertices = [
             0.0,  1.0,  0.0,
            -1.0, -1.0,  0.0,
             1.0, -1.0,  0.0
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
        triangleVertexPositionBuffer.itemSize = 3;
        triangleVertexPositionBuffer.numItems = 3;

        squareVertexPositionBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
        vertices = [
             1.0,  1.0,  0.0,
            -1.0,  1.0,  0.0,
             1.0, -1.0,  0.0,
            -1.0, -1.0,  0.0
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
        squareVertexPositionBuffer.itemSize = 3;
        squareVertexPositionBuffer.numItems = 4;
}


function drawScene() {
        gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

        mat4.identity(mvMatrix);

        mat4.translate(mvMatrix, [transX, transY, zoom]);
        mat4.rotate(mvMatrix, rotation, [0,0,1]);

        gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
        gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
        setMatrixUniforms();
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, squareVertexPositionBuffer.numItems);
}



function webGLStart() {
        var canvas = document.getElementById("gl-canvas");
        initGL(canvas);
        initShaders();
        initBuffers();

        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.enable(gl.DEPTH_TEST);

    	document.onkeydown = handleKeyDown;
    	document.onkeyup = handleKeyUp;

        tick();
}

function tick() {
	requestAnimFrame(tick);
	handleKeys();
	drawScene();	
}
	
var currentlyPressedKeys = {};

function handleKeyUp(event) {
	currentlyPressedKeys[event.keyCode] = false;
}

function handleKeyDown(event){
  	currentlyPressedKeys[event.keyCode] = true;
}
	
function zoomIn(){
    if (zoom+1 >=  m_maxZ) {
    	zoom = m_maxZ;
    }else{
    	zoom += 0.01;
    }
}

function zoomOut(){
    if (zoom <=  m_minZ) {
        zoom = m_minZ;
    }else{
    	zoom -= 0.01;
	}
}

function panUp()   { transY += 0.1; }
function panDown() { transY -= 0.1; }
function panLeft() { transX -= 0.1; }
function panRight(){ transX += 0.1; }
function rotateLeft() { rotation -= 0.03; }
function rotateRight() { rotation += 0.03; }
	
	
function reset() {
	transY = 0.0;
	transX = 0.0;
	rotation = 0.0;
	zoom = -7.0;
}
	
function handleKeys() {
	if(currentlyPressedKeys[33]) { // Page Up
   		zoomIn();
	}
   	if(currentlyPressedKeys[34]){ // Page Down
    	zoomOut()
	}
	if(currentlyPressedKeys[37]){ // LEft 
		panLeft();
	}
	if(currentlyPressedKeys[38]){ // UP
		panUp();
	}
	if(currentlyPressedKeys[39]){ // RIGHT
		panRight();
	}
	if(currentlyPressedKeys[40]){ // DOWN
		panDown();
	}
	if(currentlyPressedKeys[35]){ // END
		rotateRight();
	}
	if(currentlyPressedKeys[36]){ // HOME
		rotateLeft();
	}
}

window.setTimeout(tick, 1000);