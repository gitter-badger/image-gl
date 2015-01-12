
var squareVertexTextureCoordBuffer

var tilePositionBufferGrid;
var tileTextureGrid;

function initBuffersAndTextures(){
	initTextureFrameBuffer();
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

var framebufferVertexPositionBuffer;

function initBuffers() {
	var rows = tileImage.rows ;
	var cols = tileImage.cols ;

	tilePositionBufferGrid = Create2DArray(rows, cols);

	// Make tile buffer:
	for(var row = 0; row < rows; row++){
		for(var col = 0; col < cols; col++){
			
			var tRow = (rows - row)  - (rows / 2.0);
			var tCol = (cols - col) - (cols / 2.0);
			var tile = [
				tCol  , tRow  ,
				tCol-1, tRow  ,
				tCol-1, tRow-1,
				tCol  , tRow-1,
				tCol  , tRow
			] 
			tilePositionBufferGrid[row][col] = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, tilePositionBufferGrid[row][col]);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tile), gl.STATIC_DRAW);
	 		tilePositionBufferGrid[row][col].itemSize = 2;
	 		tilePositionBufferGrid[row][col].numItems = 5;
		}
	}
	
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
    
    
    
    // This is the quad for the framebuffer texture to bind to
    framebufferVertexPositionBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, framebufferVertexPositionBuffer);

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
	framebufferVertexPositionBuffer.itemSize = 2;
	framebufferVertexPositionBuffer.numItems = 5;	
}

function handleLoadedGridTexture(row,col){
	try{
		handleLoadedTexture(tileTextureGrid[row][col]);
	}catch(e){
		//alert(row + " row, " + col + " col");
	}
}

var rttFramebuffer;
var rttTexture;

function initTextureFrameBuffer(){
    rttFramebuffer = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, rttFramebuffer);
    rttFramebuffer.width = tileImage.stretchWidth;
    rttFramebuffer.height = tileImage.stretchHeight;
    
    rttTexture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, rttTexture);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
     
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, rttFramebuffer.width, rttFramebuffer.height, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
     
    var renderbuffer = gl.createRenderbuffer();
    gl.bindRenderbuffer(gl.RENDERBUFFER, renderbuffer);
    gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, rttFramebuffer.width, rttFramebuffer.height);
    
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, rttTexture, 0);
    gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, renderbuffer);

    gl.bindTexture(gl.TEXTURE_2D, null);
    gl.bindRenderbuffer(gl.RENDERBUFFER, null);
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
}

function initTextures() {
		var rows = tileImage.rows ;
		var cols = tileImage.cols ;

		tileTextureGrid = Create2DArray( rows, cols );

		for(var row = 0; row < rows; row++){
			for(var col = 0; col < cols; col++){
				var r = row;
				var c = col;
				tileTextureGrid[ r ][ c ] = gl.createTexture();
				tileTextureGrid[ r ][ c ].image = new Image();
				tileTextureGrid[ r ][ c ].image.onload = function() { 
					
				}
				//tileTextureGrid[ r ][ c ].image.src = "tiles/tile_" + r + "" + c + "_" + tileImage.dimension + ".jpg";
			}
		}
 
		    tileTextureGrid[0][0].image.onload = function() { 
		    	handleLoadedGridTexture(0,0) 
		    	tileTextureGrid[0][0].image.onload = function() {
		    	 	handleLoadedGridTexture(0,0) 
		    	}
		    	tileTextureGrid[0][0].image.src = "tiles/tile_" + 0 + "" + 0 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[0][1].image.onload = function() {   
		    	handleLoadedGridTexture(0,1) 
		    	tileTextureGrid[0][1].image.onload = function() {
		    	 	handleLoadedGridTexture(0,1) 
		    	}
		    	tileTextureGrid[0][1].image.src = "tiles/tile_" + 0 + "" + 1 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[0][2].image.onload = function() { 
		    	handleLoadedGridTexture(0,2) 
		    	tileTextureGrid[0][2].image.onload = function() {
		    	 	handleLoadedGridTexture(0,2) 
		    	}
		    	tileTextureGrid[0][2].image.src = "tiles/tile_" + 0 + "" + 2 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[0][3].image.onload = function() { 
		    	handleLoadedGridTexture(0,3) 
		    	tileTextureGrid[0][3].image.onload = function() {
		    	 	handleLoadedGridTexture(0,3) 
		    	}
		    	tileTextureGrid[0][3].image.src = "tiles/tile_" + 0 + "" + 3 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[0][4].image.onload = function() { 
		    	handleLoadedGridTexture(0,4) 
		    	tileTextureGrid[0][4].image.onload = function() {
		    	 	handleLoadedGridTexture(0,4) 
		    	}
		    	tileTextureGrid[0][4].image.src = "tiles/tile_" + 0 + "" + 4 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[0][5].image.onload = function() { 
		    	handleLoadedGridTexture(0,5) 
		    	tileTextureGrid[0][5].image.onload = function() {
		    	 	handleLoadedGridTexture(0,5) 
		    	}
		    	tileTextureGrid[0][5].image.src = "tiles/tile_" + 0 + "" + 5 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
		    tileTextureGrid[1][0].image.onload = function() { 
		    	handleLoadedGridTexture(1,0) 
		    	tileTextureGrid[1][0].image.onload = function() {
		    	 	handleLoadedGridTexture(1,0) 
		    	}
		    	tileTextureGrid[1][0].image.src = "tiles/tile_" + 1 + "" + 0 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[1][1].image.onload = function() {   
		    	handleLoadedGridTexture(1,1) 
		    	tileTextureGrid[1][1].image.onload = function() {
		    	 	handleLoadedGridTexture(1,1) 
		    	}
		    	tileTextureGrid[1][1].image.src = "tiles/tile_" + 1 + "" + 1 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[1][2].image.onload = function() { 
		    	handleLoadedGridTexture(1,2) 
		    	tileTextureGrid[1][2].image.onload = function() {
		    	 	handleLoadedGridTexture(1,2) 
		    	}
		    	tileTextureGrid[1][2].image.src = "tiles/tile_" + 1 + "" + 2 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[1][3].image.onload = function() { 
		    	handleLoadedGridTexture(1,3) 
		    	tileTextureGrid[1][3].image.onload = function() {
		    	 	handleLoadedGridTexture(1,3) 
		    	}
		    	tileTextureGrid[1][3].image.src = "tiles/tile_" + 1 + "" + 3 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[1][4].image.onload = function() { 
		    	handleLoadedGridTexture(1,4) 
		    	tileTextureGrid[1][4].image.onload = function() {
		    	 	handleLoadedGridTexture(1,4) 
		    	}
		    	tileTextureGrid[1][4].image.src = "tiles/tile_" + 1 + "" + 4 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[1][5].image.onload = function() { 
		    	handleLoadedGridTexture(1,5) 
		    	tileTextureGrid[1][5].image.onload = function() {
		    	 	handleLoadedGridTexture(1,5) 
		    	}
		    	tileTextureGrid[1][5].image.src = "tiles/tile_" + 1 + "" + 5 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
		    tileTextureGrid[2][0].image.onload = function() { 
		    	handleLoadedGridTexture(2,0) 
		    	tileTextureGrid[2][0].image.onload = function() {
		    	 	handleLoadedGridTexture(2,0) 
		    	}
		    	tileTextureGrid[2][0].image.src = "tiles/tile_" + 2 + "" + 0 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[2][1].image.onload = function() {   
		    	handleLoadedGridTexture(2,1) 
		    	tileTextureGrid[2][1].image.onload = function() {
		    	 	handleLoadedGridTexture(1,1) 
		    	}
		    	tileTextureGrid[2][1].image.src = "tiles/tile_" + 2 + "" + 1 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[2][2].image.onload = function() { 
		    	handleLoadedGridTexture(2,2) 
		    	tileTextureGrid[2][2].image.onload = function() {
		    	 	handleLoadedGridTexture(2,2) 
		    	}
		    	tileTextureGrid[2][2].image.src = "tiles/tile_" + 2 + "" + 2 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[2][3].image.onload = function() { 
		    	handleLoadedGridTexture(2,3) 
		    	tileTextureGrid[2][3].image.onload = function() {
		    	 	handleLoadedGridTexture(2,3) 
		    	}
		    	tileTextureGrid[2][3].image.src = "tiles/tile_" + 2 + "" + 3 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[2][4].image.onload = function() { 
		    	handleLoadedGridTexture(2,4) 
		    	tileTextureGrid[2][4].image.onload = function() {
		    	 	handleLoadedGridTexture(2,4) 
		    	}
		    	tileTextureGrid[2][4].image.src = "tiles/tile_" + 2 + "" + 4 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }
			tileTextureGrid[2][5].image.onload = function() { 
		    	handleLoadedGridTexture(2,5) 
		    	tileTextureGrid[2][5].image.onload = function() {
		    	 	handleLoadedGridTexture(2,5) 
		    	}
		    	tileTextureGrid[2][5].image.src = "tiles/tile_" + 2 + "" + 5 + "_" + tileImage.dimension + "." + tileImage.format ;
		    }

	for(var row = 0; row < rows; row++){
		for(var col = 0; col < cols; col++){
			tileTextureGrid[row][col].image.src = "tiles/tile_" + row + "" + col + "_" + 256 + "." + tileImage.format ;
		}
	}
	
		// for(var row = 0; row < rows; row++){
		// for(var col = 0; col < cols; col++){
			// tileTextureGrid[row][col].image.src = "tiles/tile_" + row + "" + col + "_" + tileImage.dimension + "." + tileImage.format ;
		// }
	// }
}

function drawGrid(x,y,w,h){

	gl.viewport(x, y, w, h);
	
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective( 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix );

	mat4.identity( mvMatrix );

	// Rot v2, although now Pan doesn't work properly';

	mat4.rotate( mvMatrix, settings.rotation,        [0, 0, 1] ); // Rotation around Z axis

	mat4.translate( mvMatrix, [transX, transY, settings.zoom] );

	mvPushMatrix();
	mat4.rotate( mvMatrix, m_rotx,    [1, 0, 0] );
    mat4.rotate( mvMatrix, m_roty,   [0, 1, 0] ); // Animation rotate around y axis

    mat4.rotate( mvMatrix, m_animateSquare, [0.4, 0.1, 0.8] ); // Animation rotate around y axis

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

	var rows = tileImage.rows ;
	var cols = tileImage.cols ;

	try{
		
		for(var row = 0; row < rows; row++){
			for(var col = 0; col < cols; col++){
			    gl.bindBuffer(gl.ARRAY_BUFFER, tilePositionBufferGrid[row][col]);
			    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, tilePositionBufferGrid[row][col].itemSize, gl.FLOAT, false, 0, 0);
			
				gl.activeTexture(gl.TEXTURE0);
			    gl.bindTexture(gl.TEXTURE_2D, tileTextureGrid[row][col]);
			    gl.uniform1i(shaderProgram.samplerUniform, 0);	
			    
			    setMatrixUniforms();
				gl.drawArrays(gl.TRIANGLE_STRIP, 0, tilePositionBufferGrid[row][col].numItems);
				
				if(row * col %2  == 0){
    				mat4.rotate( mvMatrix, m_animateEven, [0.4, 0.5, 1.0] );
    			}else{
    				mat4.rotate( mvMatrix, m_animateOdd, [0.6, 0.8, 0.2] ); 
    			}
			}
		}
	}catch(e){
		//alert(e);
	}
	mvPopMatrix();	
}

function drawFrameBufferTexture(x,y,w,h) {
	gl.viewport(x,y,w,h);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	mat4.perspective( 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix );

	mat4.identity( mvMatrix );

	mvPushMatrix();

    gl.bindBuffer(gl.ARRAY_BUFFER, framebufferVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, framebufferVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

	gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, rttTexture);
    gl.uniform1i(shaderProgram.samplerUniform, 0);


	setMatrixUniforms();
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, framebufferVertexPositionBuffer.numItems);

	mvPopMatrix();
}

function drawScene() {
	// gl.bindFramebuffer(gl.FRAMEBUFFER, rttFramebuffer);
	// drawGrid(0, 0, rttFramebuffer.width, rttFramebuffer.height);
	// gl.bindFramebuffer(gl.FRAMEBUFFER, null);
	// drawFrameBufferTexture(0, 0, gl.viewportWidth, gl.viewportHeight);

	drawGrid(0, 0, gl.viewportWidth, gl.viewportHeight);

}