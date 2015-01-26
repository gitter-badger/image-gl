
var squareVertexTextureCoordBuffer;
var tilePositionBufferGrid;
var tileTextureGrid;

var bcgColorBuffer;

function initBuffersAndTextures(){

	initBuffers();

    // There are no textures
}

function Create2DArray(rows,columns) {
   var x = new Array(rows);
   for (var i = 0; i < rows; i++) {
       x[i] = new Array(columns);
   }
   return x;
}

var tilePositionBufferGrid;
var tilePoints;

function initBuffers() {

}


function initTextures() {

}

function drawShapes(x,y,w,h){
	
	gl.viewport(x, y, w, h);
	
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective( 45, w / h, 0.1, 100.0, pMatrix );

	mat4.identity( mvMatrix );

	// Rot v2, although now Pan doesn't work properly';

	mat4.rotate( mvMatrix, settings.rotation,        [0, 0, 1] ); // Rotation around Z axis
	mat4.translate( mvMatrix, [transX, transY, zoomZ] );

    mvPushMatrix();
    mat4.rotate( mvMatrix, m_rotx,          [1, 0, 0] );
    mat4.rotate( mvMatrix, m_roty,          [0, 1, 0] );       // Animation rotate around y axis
    mat4.rotate( mvMatrix, m_animateSquare, [0.4, 0.1, 0.8] ); // Animation rotate around y axis

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, bcgColorBuffer);
	gl.vertexAttribPointer(shaderProgram.uBCG, bcgColorBuffer.itemSize, gl.FLOAT, false, 0, 0);

	mvPopMatrix();	
}
function drawScene() {
	updateBCG(m_brightness, m_contrast, m_gamma);
	drawShapes(0, 0, gl.viewportWidth, gl.viewportHeight);
	
	//drawLines();
}
