
// Small image, single tile

var squareVertexPositionBuffer;
var squareVertexTextureCoordBuffer

function initBuffersAndTextures(){
	initBuffers();
	initTextures();	
}

function initBuffers() {

	squareVertexPositionBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);

	var aspectRatio = tileImage.width / tileImage.height;

	if(aspectRatio > 1.0){
		 vertices = [
			 -aspectRatio / 2,  0.5,
			  aspectRatio / 2,  0.5,
			  aspectRatio / 2, -0.5,
			 -aspectRatio / 2, -0.5,
			 -aspectRatio / 2,  0.5
		 ];
	}else{
		 vertices = [
			 -0.5, aspectRatio,
			  0.5, aspectRatio,
			  0.5,-aspectRatio,
			 -0.5,-aspectRatio,
			 -0.5, aspectRatio
		 ];
 	}

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
	squareVertexPositionBuffer.itemSize = 2;
	squareVertexPositionBuffer.numItems = 5;

	squareVertexTextureCoordBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
	var textureCoords = [
          0.0, 1.0,
          1.0, 1.0,
          1.0, 0.0,
          0.0, 0.0,
          0.0, 1.0
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoords), gl.STATIC_DRAW);
    squareVertexTextureCoordBuffer.itemSize = 2;
    squareVertexTextureCoordBuffer.numItems = 5;
}

var mytexture;
var imgSrc = "tiles/tile_13_1024.jpg";

function initTextures() {
	try{
    	mytexture = gl.createTexture();
    	mytexture.image = new Image();
    	mytexture.image.onload = function() {
			handleLoadedTexture(mytexture)
    	}

    	mytexture.image.src = imgSrc;
   }catch(e){
   		alert(e);
   }
}


function drawScene() {
	gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective( 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix );

	mat4.identity( mvMatrix );

	// Rot v2, although now Pan doesn't work properly';

	mat4.rotate( mvMatrix, settings.rotation,        [0, 0, 1] ); // Rotation around Z axis

	mat4.translate( mvMatrix, [transX, transY, settings.zoom] );

	mvPushMatrix();
	mat4.rotate( mvMatrix, m_rotx,    [1, 0, 0] );
    mat4.rotate( mvMatrix, m_roty,   [0, 1, 0] ); // Animation rotate around y axis

   	if(m_animateOn){
    	mat4.rotate( mvMatrix, m_animateSquare, [0.4, 0.5, 0.8] ); // Animation rotate around y axis
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

	gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, mytexture);
    gl.uniform1i(shaderProgram.samplerUniform, 0);


	setMatrixUniforms();
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, squareVertexPositionBuffer.numItems);

	mvPopMatrix();
}

