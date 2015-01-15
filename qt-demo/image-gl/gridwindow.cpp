#include "gridwindow.h"
#include "imagegrid.h"
#include "imagetile.h"
#include <QScreen>
#include <QDateTime>

// constant rotation around y axis at m_flipfreq * 10
GLfloat m_animateSquare = 0;
GLboolean m_animateOn = false;

// zoom range & init
const GLfloat m_maxZ = -0.1;
const GLfloat m_minZ = -20.0;
const GLfloat m_initZ = -8.0;
const GLfloat m_stepZ = 5.0;

const GLfloat initBrightness = 0;
const GLfloat initContrast = 100;
const GLfloat initGamma = 50;

const GLfloat minBrightness = -100;
const GLfloat maxBrightness = 100;
const GLfloat minContrast = 0;
const GLfloat maxContrast = 200;
const GLfloat minGamma = 0;
const GLfloat maxGamma = 100;

const GLfloat pi = 3.14159265359;

static const char *vertexShaderSource =
        "attribute vec3 aVertexPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "varying vec2 vTextureCoord;\n"
        "void main(void) {\n"
          "gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);\n"
          "vTextureCoord = aTextureCoord;\n"
        "}\n";

//        "precision mediump float;\n"
static const char *fragmentShaderSource =
        "varying vec2 vTextureCoord;\n"
        "uniform int uInvert;\n"
        "uniform sampler2D uSampler;\n"
        "uniform vec4 uBCG;"
        "void main(void) {\n"
          "vec4 vColor;\n"
          "vColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));\n"
          "mediump float brightness = uBCG.x;\n"
          "mediump float contrast = uBCG.y;\n"
          "mediump float gamma = uBCG.z;\n"
          "mediump float b = brightness / 100.0;\n"
          "mediump float c = contrast / 100.0;\n"
          "mediump float g;\n"
          "if (gamma > 50.0) {\n"
              "g = 1.0 + (gamma - 50.0) / 10.0;\n"
          "} else {"
              "g = 1.0 / (1.0 + (50.0 - gamma) / 10.0);\n"
          "}\n"
          "mediump float bias = (1.0 - c) / 2.0 + b * c;\n"
          "vColor.x = (pow(((vColor.x * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
          "vColor.y = (pow(((vColor.y * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
          "vColor.z = (pow(((vColor.z * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
          "if(uInvert > 0){\n"
              "vColor.x = 1.0 - vColor.x;\n"
              "vColor.y = 1.0 - vColor.y;\n"
              "vColor.z = 1.0 - vColor.z;\n"
          "}\n"
          "if(vColor.a == 0.0){\n"
              "vColor.x = 0.0;\n"
              "vColor.y = 0.0;\n"
              "vColor.z = 0.0;\n"
              "vColor.a = 1.0;\n"
          "}\n"
          "gl_FragColor = vColor;\n"
        "}\n";

// gl-shader.js
qint64 GridWindow::_tileIndex(qint64 row, qint64 col){
    qint64 cols = m_imagegrid->cols();
    qint64 index = cols * row + col;
    return index;
}

GLuint GridWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

GLuint GridWindow::gridBuffer(qint64 row, qint64 col){
    qint64 index = _tileIndex(row, col);
    return tilePositionBufferGrid.at(index);
}

GLuint GridWindow::gridTexture(qint64 row, qint64 col){
    qint64 index = _tileIndex(row, col);
    return tileTextureGrid.at(index);
}

GridWindow::GridWindow()
    : m_program(0),
      m_frame(0),
      m_flipfreq(4000.0),
      m_rotx(0),
      m_roty(0),
      m_rotz(0),
      lastTime(0),
      m_animateEven(0),
      m_animateOdd(0)
{
}

GridWindow::~GridWindow()
{
    qint64 texCount = m_imagegrid->rows() * m_imagegrid->cols();
    glDeleteTextures(texCount, m_uTileTextures);
}

void GridWindow::render()
{
    drawScene();
}

void GridWindow::initialize()
{
    webGLStart();
}

// gl-bp.js
void GridWindow::webGLStart() {

    initGL();
    initShaders();
    initBuffersAndTextures();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

}

// gl-bp.js
void GridWindow::setMatrixUniforms(){
    m_program->setUniformValue(pMatrixUniform, pMatrix);
    m_program->setUniformValue(pMatrixUniform, mvMatrix);
}

// gl-bp.js
void GridWindow::initGL()
{
    // Set viewport dimension
    glViewport(0,0,width(),height());
}

// gl-bp.js
void GridWindow::handleLoadedTexture(QImage image, GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
//    glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, ) // Flip missing
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// controls.js
void GridWindow::resetSettings(){
    m_animateSquare = 0;
    m_animateEven = 0;
    m_animateOdd = 0;
    m_animateOn = false;

    settings.invert = false;
    settings.brightness = initBrightness;
    settings.contrast = initContrast;
    settings.gamma = initGamma;

    updateBCG(settings.brightness, settings.contrast, settings.gamma);
    updateInvert(settings.invert);
}

// controls.js
void GridWindow::reset(){
    resetSettings();

    settings.transY = 0.0;
    settings.transX = 0.0;

    settings.rotation = 0.0;
    settings.zoom = m_initZ;
    settings.flipH = false;
    settings.flipV = false;
}

// gl-shader.js
void GridWindow::initShaders(){
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    vertexPositionAttribute         = m_program->attributeLocation("aVertexPosition");
    textureCoordAttribute           = m_program->attributeLocation("aTextureCoord");
    mvMatrixUniform                 = m_program->uniformLocation( "uMVMatrix" );
    pMatrixUniform                  = m_program->uniformLocation( "uPMatrix" );

    uBCG 				            = m_program->uniformLocation( "uBCG" );
    uInvert 			            = m_program->uniformLocation( "uInvert" );

    pMatrixUniform 	                = m_program->uniformLocation( "uPMatrix" );
    mvMatrixUniform 	            = m_program->uniformLocation( "uMVMatrix" );
    samplerUniform 	                = m_program->uniformLocation( "uSampler" );

    colorUniform 		            = m_program->uniformLocation( "uColor");
}

// gl-shader.js
void GridWindow::updateBCG( GLfloat brightness, GLfloat contrast, GLfloat gamma ) {
    glUniform4f(uBCG, brightness, contrast, gamma , 1.0);
}

// gl-shader.js
void GridWindow::updateInvert( bool invert ){
    if(invert){
        glUniform1i(uInvert, 1);
    }else{
        glUniform1i(uInvert, 0);
    }
}

// grid.js
void GridWindow::initTextures(){
//    int rows = tileImage.rows;
//    int cols = tileImage.cols;

    /// TODO: ImageGrid to load tiles & call back to GridWindow: handleLoadedGridTexture(row, column)
}

// grid.js
void GridWindow::drawScene(){
    updateBCG(settings.brightness, settings.contrast, settings.gamma);
    drawGrid(0,0,width(),height());
}

// grid.js
void GridWindow::drawGrid(int x, int y, int w, int h){
    glViewport(0,0,width(),height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //TODO: Perspective Transform on pMatrix

    //TODO: matrix stack

    glLoadIdentity();

    glRotatef( settings.rotation, 0, 0, 1); // Z rotation (viewport)

    //TODO: Translate

//    rotate(m_rotx, 1, 0, 1 ); // for Horizontal rotation
//    rotate(m_roty, 0, 1, 0 ); // for Vertical rotation

    // pushMatrix

    // For Animation
//    rotate(m_animateSquare, 0.4, 0.1, 0.8);

    glBindBuffer(GL_ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    glVertexAttribPointer(textureCoordAttribute, 2, GL_FLOAT, false, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, bcgColorBuffer);
    glVertexAttribPointer(uBCG, 4, GL_FLOAT, false, 0, 0);


    int rows = m_imagegrid->rows();
    int cols = m_imagegrid->cols();

    for( int row = 0; row < rows; row++ ) {
        for( int col = 0; col < cols; col++ ) {
            int tileIndex = _tileIndex(row, col);
            glBindBuffer(GL_ARRAY_BUFFER, tilePositionBufferGrid.at(tileIndex));
            glVertexAttribPointer(vertexPositionAttribute, 5, GL_FLOAT, false, 0, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tileTextureGrid.at(_tileIndex(row,col)));
            setMatrixUniforms();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

            // For "A" animation
            if(row * col % 2 == 0){
                glRotatef(m_animateEven, 1.0, 0.4, 0.6);
            }else{
                glRotatef(m_animateOdd, 0.3, 1.0, 0.2);
            }
        }
    }
    // popMatrix
}


// grid.js
void GridWindow::initBuffers(){
    int rows = m_imagegrid->rows();
    int cols = m_imagegrid->cols();

    // Init tile grid
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){

            int tRow = (rows - row)  - (rows / 2.0);
            int tCol = ( col ) - (cols / 2.0);
            if(cols / 2.0 == 0){
                tCol += 0.5;
            }else{
                tCol += 1.0;
            }

            int tile[10] = {
                tCol-1 , tRow  ,
                tCol   , tRow  ,
                tCol   , tRow-1,
                tCol-1 , tRow-1,
                tCol-1 , tRow
            };


            qint64 tileIndex = _tileIndex(row, col);

            glBindBuffer( GL_ARRAY_BUFFER, tilePositionBufferGrid.at(tileIndex) );
            glBufferData( GL_ARRAY_BUFFER, 2 * 5,  tile, GL_STATIC_DRAW );
        }
    }


    // Init tile buffer
    GLuint squareVertexTextureCoordBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, squareVertexTextureCoordBuffer);
    int tile[10] = {
          0.0, 1.0,
          1.0, 1.0,
          1.0, 0.0,
          0.0, 0.0,
          0.0, 1.0
    };

    glBufferData(GL_ARRAY_BUFFER, 2 * 5, tile, GL_STATIC_DRAW);

    // Init color buffer
    puBCG = (void *)malloc(4 * 1);
    glBindBuffer(GL_ARRAY_BUFFER, bcgColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 1, puBCG, GL_STATIC_DRAW);
}

// grid.js
void GridWindow::initTextures(qint64 tileCount){
    m_uTileTextures = (GLuint *)malloc( sizeof(GLuint) * tileCount );
}

// grid.js
void GridWindow::initBuffersAndTextures(){
    initBuffers();
    initTextures();
}

// grid.js
void GridWindow::handleLoadedGridTexture( int row, int col ){
    ImageTile *tile = m_imagegrid->tile(row,col);
    handleLoadedTexture(tile->image(), tileTextureGrid.at( _tileIndex( row,col )));
}


// Animate is called in tick()
void GridWindow::animate() {
    QDateTime dt;
    qint64 timeNow = dt.currentMSecsSinceEpoch();

    if (lastTime != 0) {
        qint64 elapsed = timeNow - lastTime;

        if(m_animateOn){
            m_animateSquare += (75 * elapsed) / (m_flipfreq * 50) ;
            m_animateEven += (75 * elapsed) / (m_flipfreq * 40) ;
            m_animateOdd += (75 * elapsed) / (m_flipfreq * 20) ;
        }else{
            if(m_animateSquare > 0){
                m_animateSquare -= (75 * elapsed) / (m_flipfreq * 5) ;
                if(m_animateSquare < 0){
                    m_animateSquare = 0;
                }
            }
            if(m_animateEven > 0){
                m_animateEven -= (75 * elapsed) / (m_flipfreq * 4) ;
                if(m_animateEven < 0){
                    m_animateEven = 0;
                }
            }
            if(m_animateOdd > 0){
                m_animateOdd -= (75 * elapsed) / (m_flipfreq * 2) ;
                if(m_animateOdd < 0){
                    m_animateOdd = 0;
                }
            }
        }

        if(settings.flipH){
            // increase from 0 to pi
            m_rotx += (75 *elapsed) / m_flipfreq;
            if(m_rotx > pi){
                m_rotx = pi;
            }
        }else{
            // decrease from pi to 0
            m_rotx -= (75 * elapsed) / m_flipfreq;
            if(m_rotx < 0.0){
                m_rotx = 0.0;
            }
        }
        if(settings.flipV){
            // increase from 0 to pi
            m_roty += (75 * elapsed) / m_flipfreq;
            if(m_roty > pi){
                m_roty = pi;
            }
        }else{
            // decrease from pi to 0
            m_roty -= (75 * elapsed) / m_flipfreq;
            if(m_roty < 0.0){
                m_roty = 0.0;
            }
        }

        if(m_rotz < settings.rotation){
            m_rotz += (75 * elapsed) / m_flipfreq;
            if(m_rotz > settings.rotation){
                // rotated too far
                m_rotz = settings.rotation;
            }
        }
        if(m_rotz > settings.rotation){
            m_rotz -= (75 * elapsed) / m_flipfreq;
            if(m_rotz > settings.rotation){
                // rotated too far
                m_rotz = settings.rotation;
            }
        }
    }
    lastTime = timeNow;
}

void GridWindow::setGrid(ImageGrid *grid)
{
    m_imagegrid = grid;
}
