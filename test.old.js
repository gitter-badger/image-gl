var gl;

var pi = 3.14159265359;

var m_flipH = false;
var m_flipV = false; 
function flipH() {
	m_flipH = !m_flipH;
}
function flipV() {
	m_flipV = !m_flipV;
}
var m_rotx = 0;
var m_roty = 0;

var m_animateSquare = 0;

var m_maxZ = -0.1;
var m_minZ = -20.0;
var m_initZ = -3.0;
var m_antiAliasingEnabled = false;

var m_commandKeyPressed = false;

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
var squareVertexTextureCoordBuffer

var mvMatrix = mat4.create();
var mvMatrixStack = [];
var pMatrix = mat4.create();


var mouseDown = false;
var mouseDownRight = false;
var lastMouseX = null;
var lastMouseY = null;

var moonRotationMatrix = mat4.create();
mat4.identity(moonRotationMatrix);

function handleMouseDown(e) {

	if (!e.which && e.button) {
		if (e.button & 1) e.which = 1      // Left
    	else if (e.button & 4) e.which = 2 // Middle
		else if (e.button & 2) e.which = 3 // Right
	}
	if(e.which == 1){
    	mouseDown = true;
    }
    if(e.which == 3){
    	mouseDownRight = true;
    }
    lastMouseX = e.clientX;
    lastMouseY = e.clientY;
}

function handleMouseUp(e) {
		if (!e.which && e.button) {
		if (e.button & 1) e.which = 1      // Left
    	else if (e.button & 4) e.which = 2 // Middle
		else if (e.button & 2) e.which = 3 // Right
	}
	if(e.which == 1){
    	mouseDown = false;
    }
    if(e.which == 3){
    	mouseDownRight = false;
    }
}

function handleMouseMove(event) {
    if (!mouseDown) {
      return;
    }
    var newX = event.clientX;
    var newY = event.clientY;

    var deltaX = newX - lastMouseX;
    var deltaY = newY - lastMouseY;
    
    if(m_commandKeyPressed){

	    if(deltaX > 0){
	    	for(var i = 0; i < deltaX; i++){
	    		panRight();
	    	}
	    }else{
	    	for(var i = 0; i < -deltaX; i++){
	    		panLeft();
	    	}
	    }
	
	    if(deltaY > 0){
	    	for(var i = 0; i < deltaY; i++){
	    		
	    		panDown();
	    	}
	    }else{
	    	for(var i = 0; i < -deltaY; i++){
	    		panUp();
	    	}
	    }
    }

    lastMouseX = newX
    lastMouseY = newY;
}


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
    
    shaderProgram.textureCoordAttribute = gl.getAttribLocation(shaderProgram, "aTextureCoord");
    gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);	

	shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
	shaderProgram.samplerUniform = gl.getUniformLocation(shaderProgram, "uSampler");
}



function setMatrixUniforms() {
	gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
	gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}




function initBuffers() {
	squareVertexPositionBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
	vertices = [
         1.0,  1.0,
        -1.0,  1.0, 
         1.0, -1.0, 
        -1.0, -1.0, 
	];
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
	squareVertexPositionBuffer.itemSize = 2;
	squareVertexPositionBuffer.numItems = 4;
	
	squareVertexTextureCoordBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
	var textureCoords = [
          0.0, 0.0,
          0.0, 1.0,
          1.0, 1.0,
          1.0, 0.0,
    ];
   
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoords), gl.STATIC_DRAW);
    squareVertexTextureCoordBuffer.itemSize = 2;
    squareVertexTextureCoordBuffer.numItems = 4;
}


function drawScene() {
	gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

	mat4.identity(mvMatrix);

	mat4.translate(mvMatrix, [transX, transY, zoom]);
	
	mvPushMatrix();
	mat4.rotate( mvMatrix, m_rotx,    [1, 0, 0] );
    mat4.rotate( mvMatrix, m_roty,   [0, 1, 0] ); // Animation rotate around y axis
	mat4.rotate( mvMatrix, rotation,  [0, 0, 1] ); // Rotation around Z axis

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
    
    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

	gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, mytexture);
    gl.uniform1i(shaderProgram.samplerUniform, 0);
	
	setMatrixUniforms();
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, squareVertexPositionBuffer.numItems);
	
	mvPopMatrix();
}

var lastTime = 0;
function animate() {
    var timeNow = new Date().getTime();
    if (lastTime != 0) {
		var elapsed = timeNow - lastTime;
		//m_animateSquare += (75 * elapsed) / 100000.0;

		if(m_flipH){ 
			// increase from 0 to pi
			m_rotx += (75 * elapsed) / 10000.0;
			if(m_rotx > pi)
				m_rotx = pi;
		}else{
			// decrease from pi to 0
			m_rotx -= (75 * elapsed) / 10000.0;
			if(m_rotx < 0.0)
				m_rotx = 0.0;
				
		}
		if(m_flipV){ 
			// increase from 0 to pi
			m_roty += (75 * elapsed) / 10000.0;
			if(m_roty > pi)
				m_roty = pi;
		}else{
			// decrease from pi to 0
			m_roty -= (75 * elapsed) / 10000.0;
			if(m_roty < 0.0)
				m_roty = 0.0;
				
		}	

    }
	lastTime = timeNow;
}

function webGLStart() {
	var canvas = document.getElementById("gl-canvas");
	initGL(canvas);
	initShaders();
	initBuffers();
	initTexture();

	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	gl.enable(gl.DEPTH_TEST);

    canvas.onmousedown = handleMouseDown;
    document.onmouseup = handleMouseUp;
    document.onmousemove = handleMouseMove;

	document.onkeydown = handleKeyDown;
	document.onkeyup = handleKeyUp;

	tick();
}

var mytexture;
function initTexture() {
	try{
    	mytexture = gl.createTexture();
    	mytexture.image = new Image();
    	mytexture.image.onload = function() {    	
			handleLoadedTexture(mytexture)
    	}
    
    	mytexture.image.src = "2.png";
   }catch(e){
   		alert(e);
   }
}

function handleLoadedTexture(texture) {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    
    // Used for claming textures that are not power of 2
    gl.texParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	gl.texParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
    gl.bindTexture(gl.TEXTURE_2D, null);
}

function tick() {
	requestAnimFrame(tick);
	handleKeys();
	drawScene();
	animate();	
}
	
var currentlyPressedKeys = {};

function mouseWheelHandler(e){
	// cross-browser wheel delta
	 if(m_commandKeyPressed){
		var e = window.event || e; // old IE support
		var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
	
		zoom += delta / 10.0;
		if(zoom > m_maxZ){
			zoom = m_maxZ;
		}
	}
}

/// TODO: this is platform and browser specific. 
function commandKey(){
	
	// chrome + windows: alt = 19
	return 18;
	
	//91: command key in chrome & safari on mac. doesn't work in firefox
}

function isCommandKeyDown(){
	return currentlyPressedKeys[commandKey()];
}

/// TODO: event seems to only be handled incorrectly for multiple keys up. 
/// Example, holding command while rotation is applied then releasing rotate causes indefinite spin
function handleKeyUp(event) {
	var cmnd = commandKey();
	
	if( event.keyCode == cmnd ){
		m_commandKeyPressed = false;
	}
	currentlyPressedKeys[event.keyCode] = false;
}

function handleKeyDown(event){
	var cmnd = commandKey();

	if( event.keyCode == cmnd ){
		m_commandKeyPressed = true;
	}
  	currentlyPressedKeys[event.keyCode] = true;
}
	
function zoomIn(){
    if (zoom >=  m_maxZ) {
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

var panBase = 0.0018; // mac chrome was 20

function panUp()   { transY += -zoom * panBase; }
function panDown() { transY -= -zoom * panBase; }
function panLeft() { transX -= -zoom * panBase; }
function panRight(){ transX += -zoom * panBase; }
function rotateLeft() { rotation -= 0.03; }
function rotateRight() { rotation += 0.03; }
	
	
function reset() {
	transY = 0.0;
	transX = 0.0;
	rotation = 0.0;
	zoom = m_initZ;
	m_flipH = false;
	m_flipV = false;
}

/*
  This function provides a method for loading webgl textures using a pool of
  image elements, which has very low memory overhead. For more details see:
  http://blog.tojicode.com/2012/03/javascript-memory-optimization-and.html
*/
  var loadImgTexture = (function createTextureLoader() {
  var MAX_CACHE_IMAGES = 16;

  var textureImageCache = new Array(MAX_CACHE_IMAGES);
  var cacheTop = 0;
  var remainingCacheImages = MAX_CACHE_IMAGES;
  var pendingTextureRequests = [];

  var TextureImageLoader = function(loadedCallback) {
  var self = this;
  var blackPixel = new Uint8Array([0, 0, 0]);

  this.gl = null;
  this.texture = null;
  this.callback = null;

  this.image = new Image();
  this.image.addEventListener('load', function() {
  
	var gl = self.gl;
	gl.bindTexture(gl.TEXTURE_2D, self.texture);

	var startTime = Date.now();
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, self.image);

	if (isPowerOfTwo(self.image.width) && isPowerOfTwo(self.image.height)) {
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
		gl.generateMipmap(gl.TEXTURE_2D);
	} else {
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	}
	var uploadTime = Date.now() - startTime;

	if(self.callback) {
		var stats = {
			width: self.image.width,
            height: self.image.height,
            internalFormat: gl.RGBA,
            levelZeroSize: self.image.width * self.image.height * 4,
            uploadTime: uploadTime
          };
          self.callback(self.texture, null, stats);
	}
	loadedCallback(self);
	}, false);
		this.image.addEventListener('error', function(ev) {
		clearOnError(self.gl, 'Image could not be loaded', self.texture, self.callback);
		loadedCallback(self);
	}, false);
	};

    TextureImageLoader.prototype.loadTexture = function(gl, src, texture, callback) {
      this.gl = gl;
      this.texture = texture;
      this.callback = callback;
      this.image.src = src;
    };

    var PendingTextureRequest = function(gl, src, texture, callback) {
      this.gl = gl;
      this.src = src;
      this.texture = texture;
      this.callback = callback;
    };

    function releaseTextureImageLoader(til) {
      var req;
      if(pendingTextureRequests.length) {
        req = pendingTextureRequests.shift();
        til.loadTexture(req.gl, req.src, req.texture, req.callback);
      } else {
        textureImageCache[cacheTop++] = til;
      }
    }

    return function(gl, src, texture, callback) {
      var til;

      if(cacheTop) {
        til = textureImageCache[--cacheTop];
        til.loadTexture(gl, src, texture, callback);
      } else if (remainingCacheImages) {
        til = new TextureImageLoader(releaseTextureImageLoader);
        til.loadTexture(gl, src, texture, callback);
        --remainingCacheImages;
      } else {
        pendingTextureRequests.push(new PendingTextureRequest(gl, src, texture, callback));
      }

      return texture;
    };
})();

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