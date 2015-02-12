
var squareVertexTextureCoordBuffer;
var stencilVertexCoordBuffer;
var vertexPositionBuffer;
var tilePositionBufferGrid;
var tileTextureGrid;

var bcgColorBuffer;

function initBuffersAndTextures(){

	initBuffers();
	initTextures();	
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
	var rows = tileImage.rows ;
	var cols = tileImage.cols ;

	// Init tile grid
	tilePositionBufferGrid = Create2DArray(rows, cols);
	for(var row = 0; row < rows; row++){
		for(var col = 0; col < cols; col++){
			
			var tRow = (rows - row)  - (rows / 2.0);
			var tCol = ( col ) - (cols / 2.0);
			if(cols / 2.0 == 0){
				tCol += 0.5;
			}else{
				tCol += 1.0;
			}
			
			var tile = [
				tCol-1 , tRow  ,
				tCol   , tRow  ,
				tCol   , tRow-1,
				tCol-1 , tRow-1,
				tCol-1 , tRow
			] 
			tilePositionBufferGrid[row][col] = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, tilePositionBufferGrid[row][col]);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tile), gl.STATIC_DRAW);
	 		tilePositionBufferGrid[row][col].itemSize = 2;
	 		tilePositionBufferGrid[row][col].numItems = 5;
		}
	}

    stencilVertexCoordBuffer = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, stencilVertexCoordBuffer );
    var stencilCoords = [
        -1.6, 1.2,
        -1.4, -1.0,
        1.8, -1.5,
        1.0, 2.0
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(stencilCoords), gl.STATIC_DRAW);
    stencilVertexCoordBuffer.itemSize = 2;
    stencilVertexCoordBuffer.numItems = 4;
	
	// Init tile buffer
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
    
	// Init color buffer
	bcgColorBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, bcgColorBuffer);

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(uBCG), gl.STATIC_DRAW);
	bcgColorBuffer.itemSize = 4;
	bcgColorBuffer.numItems = 1;
}

function handleLoadedGridTexture(row,col){
	try{
		handleLoadedTexture(tileTextureGrid[row][col]);
	}catch(e){
		//alert(row + " row, " + col + " col");
	}
}

function initTextures() {
	var rows = tileImage.rows ;
	var cols = tileImage.cols ;

	tileTextureGrid = Create2DArray( rows, cols );

	for(var row = 0; row < rows; row++){
		for(var col = 0; col < cols; col++){
			tileTextureGrid[ row ][ col ] = gl.createTexture();
			tileTextureGrid[ row ][ col ].image = new Image();
			tileTextureGrid[ row ][ col ].image.onload = (function(nr,nc) {
				return function() {
					handleLoadedGridTexture(nr,nc);
					
					tileTextureGrid[ nr ][ nc ].image.onload = (function(nnr, nnc) {
						return function() {
							handleLoadedGridTexture(nnr,nnc);
						}
					}(nr,nc));
					tileTextureGrid[ nr ][ nc ].image.src = "tiles/tile_" + nr + "" + nc + "_" + tileImage.dimension + "." + tileImage.format;
				}
			}(row,col));
			tileTextureGrid[ row ][ col ].image.src = "tiles/tile_" + row + "" + col + "_" + tileImage.dimensionSmall + "." + tileImage.format ;
		}
	}
}

function drawGrid(x,y,w,h){

    gl.clear(gl.DEPTH_BUFFER_BIT);

    gl.enable( gl.BLEND );
    gl.enable( gl.DEPTH_TEST );
    gl.depthFunc( gl.LEQUAL );
    gl.blendFunc( gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA );


    gl.viewport(x, y, w, h);
	
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective( 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix );

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


	var rows = tileImage.rows ;
	var cols = tileImage.cols ;

	try{
		
		for(var row = 0; row < rows; row++){
			for(var col = 0; col < cols; col++){
                mvPushMatrix();
                if(row * col %2  == 0){
                    mat4.rotate( mvMatrix, m_animateEven, [1.0, 0.4, 0.6] );
                }else{
                    mat4.rotate( mvMatrix, m_animateOdd, [0.3, 1.0, 0.2] );
                }

                gl.bindBuffer(gl.ARRAY_BUFFER, tilePositionBufferGrid[row][col]);
                gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, tilePositionBufferGrid[row][col].itemSize, gl.FLOAT, false, 0, 0);

                gl.activeTexture(gl.TEXTURE0);
                gl.bindTexture(gl.TEXTURE_2D, tileTextureGrid[row][col]);
                gl.uniform1i(shaderProgram.samplerUniform, 0);
			    
			    setMatrixUniforms();
                gl.drawArrays(gl.TRIANGLE_STRIP, 0, tilePositionBufferGrid[row][col].numItems);

                mvPopMatrix();
			}
		}
		
	}catch(e){
		//alert(e);
	}
	mvPopMatrix();	
}

function _enableStencil(){
    gl.colorMask( false, false, false, false );
    gl.depthMask( false );
    gl.stencilFunc( gl.NEVER, 1, 0xFF );
    gl.stencilOp( gl.REPLACE, gl.KEEP, gl.KEEP );  // draw 1s on test fail (always)

    // draw stencil pattern
    gl.stencilMask( 0xFF);
    gl.clear( gl.STENCIL_BUFFER_BIT );  // needs mask=0xFF
    gl.enable( gl.STENCIL_TEST );
}

function _disableStencil(){
    gl.colorMask( true, true, true, true );
    gl.depthMask( true );
    gl.stencilMask( 0x00 );
    // draw where stencil's value is 0
    gl.stencilFunc( gl.EQUAL, 0, 0xFF );
    // draw only where stencil's value is 1
    gl.stencilFunc( gl.EQUAL, 1, 0xFF );
}

function drawStencil(x,y,w,h){

    _enableStencil();
    mvPushMatrix();

    gl.viewport(x, y, w, h);

    mat4.perspective( 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix );

    mat4.identity( mvMatrix );

    // Rot v2, although now Pan doesn't work properly';

    mat4.rotate( mvMatrix, settings.rotation,        [0, 0, 1] ); // Rotation around Z axis

    mat4.translate( mvMatrix, [transX, transY, zoomZ] );

    mat4.rotate( mvMatrix, m_roty,          [0, 1, 0] );       // Animation rotate around y axis
    mat4.rotate( mvMatrix, m_rotx,          [1, 0, 0] );

    /////  Draw stencil to stencil buffer
    setMatrixUniformsS();

    gl.bindBuffer(gl.ARRAY_BUFFER, stencilVertexCoordBuffer);
    gl.vertexAttribPointer(shaderProgramS.vertexPositionAttribute, 2,  gl.FLOAT, false, 0, 0);

    gl.drawArrays( gl.TRIANGLE_STRIP, 0, stencilVertexCoordBuffer.numItems );

    _disableStencil();

    mvPopMatrix();
}

function drawScene() {

    //gl.useProgram(shaderProgramS);
    //gl.enableVertexAttribArray(shaderProgramS.vertexPositionAttribute);
    //drawStencil(0,0, gl.viewportWidth, gl.viewportHeight);

    gl.useProgram(shaderProgram);
    gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);
    gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);

    updateBCG(m_brightness, m_contrast, m_gamma);
	drawGrid(0, 0, gl.viewportWidth, gl.viewportHeight);

	//drawLines();
}
