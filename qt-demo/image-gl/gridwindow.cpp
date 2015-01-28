#include "gridwindow.h"
#include "imagegrid.h"
#include "imagetile.h"
#include <QScreen>
#include <QDateTime>
#include <math.h>
#include <QApplication>
#include <iostream>

// constant rotation around y axis at m_flipfreq * 10
GLfloat m_animateSquare = 0;
GLboolean m_animateOn = false;

// zoom range & init
const GLfloat m_maxZ = -0.01;
const GLfloat m_minZ = -100.0;
const GLfloat m_initZ = -70.0;
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

const GLfloat m_rulerZ = -20.0;

const GLfloat pi = 3.14159265359;

qreal GridWindow::r2d(qreal r){
    return r * 57.2957795;
}
qreal GridWindow::d2r(qreal d){
    return d / 57.2957795;
}

GLfloat *rdColors;

void delRdColors(){
    free( rdColors );
}

void initRdColors(){

    int nVertices = 48;
    rdColors = ( GLfloat *) malloc( sizeof (GLfloat ) * nVertices * 4 );

    int j = 0;

    srand(QDateTime::currentMSecsSinceEpoch());
    for(float i = 0; i < nVertices; i++){
        float red =  1.0 / (rand() % 20);
        float green =  1.0 / (rand() % 20);
        float blue =  1.0 / (rand() % 20);

        rdColors[j++] = red;
        rdColors[j++] = green;
        rdColors[j++] = blue;
        rdColors[j++] = 0.6;
    }
}

qreal GridWindow::unitToPx(ImageGrid *grid){
    qreal val = 1.0; // Default is something crazy
    if(grid){
        qreal dx = qAbs( m_zoom / m_rulerZ);
        qreal dim = grid->dimension();
        val = dim * dx;
    }
    return val;
}

// Convert surgimap pixel position to gl position
QPointF GridWindow::sm2gl( QPointF pixPos, ImageGrid *grid ){

//    Q_ASSERT(false);
    qreal x    = pixPos.x();
    qreal y    = pixPos.y();
    qreal cols = grid->cols();
    qreal rows = grid->rows();
    qreal dim  = grid->dimension();

    x  = x / dim;
    y  = y / dim;

    x = x - (cols / 2.0);

    y = (rows / 2.0) - y;

    return QPointF( x, y );
}

// Convert gl coord position to surgimap pixel position
QPointF GridWindow::gl2sm( QPointF glPos, ImageGrid *grid ){

    qreal x    = glPos.x();
    qreal y    = glPos.y();
    qreal cols = grid->cols();
    qreal rows = grid->rows();
    qreal dim  = grid->dimension();

    if( y > 0 ){
        y = (( rows / 2.0) - y );
        y *= dim;
    }else{
        y = fabs(y) + (rows / 2.0);
        y *= dim;
    }

    if( x < 0 ){
        x = fabs(-(cols / 2.0) - x);
        x *= dim;
    }else{
        x = x + (cols / 2.0);
        x *= dim;
    }

    return QPointF( x, y );
}

struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};

//// Grid shaders
static const char *vertexShaderSourceG =
        "attribute vec2 aVertexPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "attribute vec4 aColor;\n"
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "varying vec2 vTextureCoord;\n"
        "varying vec4 vColor;\n"
        "void main(void) {\n"
        "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 0.0, 1.0);\n"
        "    vTextureCoord = aTextureCoord;\n"
        "    vColor = aColor;\n"
        "}\n";

//        "precision mediump float;\n"
static const char *fragmentShaderSourceG =
        "varying vec2 vTextureCoord;\n"
        "uniform int uInvert;\n"
        "uniform sampler2D uSampler;\n"
        "uniform vec4 uBCG;\n"
        "varying vec4 vColor;\n"
        "void main(void) {\n"
        "  vec4 oColor;\n"
        "  oColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));\n"
        "  mediump float brightness = uBCG.x;\n"
        "  mediump float contrast = uBCG.y;\n"
        "  mediump float gamma = uBCG.z;\n"
        "  mediump float b = brightness / 100.0;\n"
        "  mediump float c = contrast / 100.0;\n"
        "  mediump float g;\n"
        "  if (gamma > 50.0) {\n"
        "      g = 1.0 + (gamma - 50.0) / 10.0;\n"
        "  } else {\n"
        "      g = 1.0 / (1.0 + (50.0 - gamma) / 10.0);\n"
        "  }\n"
        "  if( oColor.x == 0.0 && oColor.y == 0.0 && oColor.z == 0.0 ){\n"
        "      oColor.w = 0.0;\n"
        "      oColor.a = 0.0;\n"
        "  }\n"
        "  mediump float bias = (1.0 - c) / 2.0 + b * c;\n"
        "  oColor.x = (pow(((oColor.x * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  oColor.y = (pow(((oColor.y * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  oColor.z = (pow(((oColor.z * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  if(uInvert > 0){\n"
        "      oColor.x = 1.0 - oColor.x;\n"
        "      oColor.y = 1.0 - oColor.y;\n"
        "      oColor.z = 1.0 - oColor.z;\n"
        "  }\n"
        "  if(oColor.w == 0.0){\n"
        "      oColor.x = 0.0;\n"
        "      oColor.y = 0.0;\n"
        "      oColor.z = 0.0;\n"
        "      oColor.w = 0.0;\n"
        "  }\n"
        "  gl_FragColor = oColor;\n"
        "}\n";

/// Hud text shaders
static const char *vertexShaderSourceT =
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "attribute vec4 aVertexPosition;\n"
        "varying vec2 vTextureCoord;\n"
        "void main(void) {\n"
        "   gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition.xy, 0, 1);\n"
        "   vTextureCoord = aVertexPosition.zw;\n"
        "}\n";

static const char *fragmentShaderSourceT =
        "varying vec2 vTextureCoord;\n"
        "uniform vec4 uColor;\n"
        "uniform sampler2D uSampler;\n"
        "void main() {\n"
        "   gl_FragColor = vec4( 1,1,1, texture2D( uSampler, vTextureCoord ).a) * uColor;\n"
        "}\n";

/// Measurement shaders
static const char *vertexShaderSourceM =
        "attribute highp vec4 aVertexPosition;\n"
        "attribute lowp vec4 aColor;\n"
        "varying lowp vec4 vColor;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   vColor = aColor;\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";
static const char *fragmentShaderSourceM =
        "varying lowp vec4 vColor;\n"
        "void main() {\n"
        "   gl_FragColor = vColor;\n"
        "}\n";

/// Stencil shader
static const char *vertexShaderSourceS =
        "attribute highp vec4 aVertexPosition;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";

/// Hud shaders
static const char *vertexShaderSourceH =
        "attribute highp vec4 aVertexPosition;\n"
        "attribute lowp vec4 aColor;\n"
        "varying lowp vec4 vColor;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   vColor = aColor;\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";
static const char *fragmentShaderSourceH =
        "varying lowp vec4 vColor;\n"
        "void main() {\n"
        "   gl_FragColor = vColor;\n"
        "}\n";


GLuint GridWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader( type );
    glShaderSource ( shader, 1, &source, 0 );
    glCompileShader ( shader );
    return shader;
}

GridWindow::GridWindow()
    :
      m_sceneRotation(QQuaternion(0,0,0,0)),
      m_frame(0),
      m_sceneProgram(0),
      m_fov(45.0f),
      m_vflip90(false),
      m_gridInitialized(false),
      m_flipfreq(4000.0),
      m_rotx(0),
      m_roty(0),
      m_rotz(0),
      lastTime(0),
      m_panBase(0.001),
      m_animateSquare(0),
      m_animateEven(0),
      m_animateOdd(0),
      m_brightness(initBrightness),
      m_contrast(initContrast),
      m_gamma(initGamma)
{
    reset();
    m_mvMatrix = QMatrix4x4();
    m_pMatrix  = QMatrix4x4();
}

GridWindow::~GridWindow()
{
    delRdColors();
    if(m_gridInitialized){

        foreach(GridImage *grid, m_GridImages){
            qint64 count = grid->m_imagegrid->rows() * grid->m_imagegrid->cols();
            glDeleteBuffers( count, grid->m_tilePositionBufferGrid);
            glDeleteBuffers( count, grid->m_tileTextureGrid);
            glDeleteBuffers( count, grid->m_tileTextureGrid2);
            glDeleteBuffers( 1, &grid->m_squareVertexTextureCoordBuffer );

            free ( grid->m_tilePositionBufferGrid );
            free ( grid->m_tileTextureGrid );
            free ( grid->m_tileTextureGrid2 );
            foreach( GridLayer *layer, grid->m_gridLayers ){
                delete layer;
            }
            delete grid;
        }
        glDeleteBuffers(1, &m_hudTextTextTexture);
        glDeleteBuffers(1, &m_hudTextVbo);
        //        free ( m_pColor );
    }
}

// Initialize FreeType
int GridWindow::_initTextResources(){
#ifdef Q_OS_OSX
    const char *fontfilename = "/Users/Jon/Downloads/FreeSans/FreeSans.ttf";
#else
#ifdef Q_OS_LINUX
    const char *fontfilename = "/home/jsuppe/Developer/SourceCodePro-ExtraLight.ttf";
#endif
#endif
    /* Initialize the FreeType2 library */
    if ( FT_Init_FreeType( &m_ft ) ) {
        qDebug() << "Could not init freetype library";
        return 0;
    }

    /* Load a font */
    if (FT_New_Face(m_ft, fontfilename, 0, &m_face)) {
        qDebug() <<  QString( "Could not open font %1\n" ).arg(fontfilename);
        return 0;
    }

    // Create the vertex buffer object
    glGenBuffers(1, &m_hudTextVbo);

    return 1;
}

qint64 GridWindow::_tileIndex(qint64 row, qint64 col, qint64 cols){
    qint64 index = cols * row + col;
    return index;
}

void GridWindow::initialize()
{
    _initTextResources();
    webGLStart();
    initRdColors();
}

void GridWindow::webGLStart() {

    initShadersScene();
    initShadersHud();
    initShadersHudText();
    initShaderMeasurements();
    initShadersStencil();

    initGridBuffersAndTextures();

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void GridWindow::initShadersScene(){
    m_sceneProgram = new QOpenGLShaderProgram( this );
    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceG );
    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceG );
    m_sceneProgram->link();

    m_sceneVertexPositionAttribute         = m_sceneProgram->attributeLocation( "aVertexPosition" );
    m_sceneTextureCoordAttribute           = m_sceneProgram->attributeLocation( "aTextureCoord" );
    m_sceneColorAttribute                  = m_sceneProgram->attributeLocation( "aColor" );

    m_sceneBCGUniform 			           = m_sceneProgram->uniformLocation( "uBCG" );
    m_sceneMVMatrixUniform                 = m_sceneProgram->uniformLocation( "uMVMatrix" );
    m_scenePMatrixUniform                  = m_sceneProgram->uniformLocation( "uPMatrix" );
    m_sceneUInvert 			               = m_sceneProgram->uniformLocation( "uInvert" );
    m_sceneSamplerUniform 	               = m_sceneProgram->uniformLocation( "uSampler" );
}

void GridWindow::initShadersHudText(){
    m_hudTextProgram = new QOpenGLShaderProgram(this);
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceT );
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceT );
    m_hudTextProgram->link();

    m_hudTextVertexPositionAttribute       = m_hudTextProgram->attributeLocation( "aVertexPosition" );
    m_hudTextColorUniform    			   = m_hudTextProgram->uniformLocation( "uColor" );
    m_hudTextPMatrixUniform                = m_hudTextProgram->uniformLocation( "uPMatrix" );
    m_hudTextMVMatrixUniform               = m_hudTextProgram->uniformLocation( "uMVMatrix" );
    m_hudTextSamplerUniform 	           = m_hudTextProgram->uniformLocation( "uSampler" );
}

void GridWindow::initShadersHud(){
    m_hudProgram = new QOpenGLShaderProgram(this);
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceH );
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceH );
    m_hudProgram->link();

    m_hudVertexPositionAttribute           = m_hudProgram->attributeLocation( "aVertexPosition" );
    m_hudColorAttribute 			       = m_hudProgram->attributeLocation( "aColor" );

    m_hudPMatrixUniform                    = m_hudProgram->uniformLocation( "uPMatrix" );
    m_hudMVMatrixUniform                   = m_hudProgram->uniformLocation( "uMVMatrix" );
}

void GridWindow::initShaderMeasurements(){
    m_measurementsProgram = new QOpenGLShaderProgram( this );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceM );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceM );
    m_measurementsProgram->link();

    m_measurementVertexPositionAttribute   = m_measurementsProgram->attributeLocation( "aVertexPosition" );
    m_measurementColorAttribute 	       = m_measurementsProgram->attributeLocation( "aColor" );
    m_measurementPMatrixUniform            = m_measurementsProgram->uniformLocation( "uPMatrix" );
    m_measurementMVMatrixUniform           = m_measurementsProgram->uniformLocation( "uMVMatrix" );
}


void GridWindow::initShadersStencil(){
    m_stencilProgram = new QOpenGLShaderProgram(this);
    m_stencilProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceS );
    m_stencilProgram->link();

    m_stencilVertexPositionAttribute      = m_stencilProgram->attributeLocation( "aVertexPosition" );
    m_stencilPMatrixUniform               = m_stencilProgram->uniformLocation( "uPMatrix" );
    m_stencilMVMatrixUniform              = m_stencilProgram->uniformLocation( "uMVMatrix" );
}

void GridWindow::setStencilMatrixUniforms(){
    m_stencilProgram->setUniformValue( m_stencilPMatrixUniform, m_pMatrix );
    m_stencilProgram->setUniformValue( m_stencilMVMatrixUniform, m_mvMatrix );
}

void GridWindow::setSceneMatrixUniforms(){
    m_sceneProgram->setUniformValue( m_scenePMatrixUniform, m_pMatrix );
    m_sceneProgram->setUniformValue( m_sceneMVMatrixUniform, m_mvMatrix );
}

void GridWindow::setHudMatrixUniforms(){
    m_hudProgram->setUniformValue( m_hudPMatrixUniform, m_pMatrix );
    m_hudProgram->setUniformValue( m_hudMVMatrixUniform, m_mvMatrix );
}

void GridWindow::setHudTextUniforms(){
    m_hudTextProgram->setUniformValue( m_hudTextPMatrixUniform,  m_pMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextMVMatrixUniform, m_mvMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextColorUniform,    m_hudTextColor);
}

void GridWindow::setMeasurementUniforms(){
    m_measurementsProgram->setUniformValue( m_measurementPMatrixUniform,  m_pMatrix );
    m_measurementsProgram->setUniformValue( m_measurementMVMatrixUniform, m_mvMatrix );
}

void GridWindow::setSceneColorUniforms(){
    updateBCG();
    updateInvert();
}

void GridWindow::updateBCG() {
    glUniform4f( m_sceneBCGUniform, m_brightness, m_contrast, m_gamma, 1.0 );
}

void GridWindow::updateInvert() {
    glUniform1i( m_sceneUInvert, m_settings.invert == true ? 1 : 0 );
}

void GridWindow::_enableStencil(){
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);  // draw 1s on test fail (always)

    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
    glEnable(GL_STENCIL_TEST);
}

void GridWindow::_disableStencil(){
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    // draw where stencil's value is 0
    glStencilFunc(GL_EQUAL, 0, 0xFF);
}

// gl-bp.js
void GridWindow::handleLoadedTexture(GridImage *grid, QImage image, GLuint texture, float dimension){

    QImage img = image.convertToFormat(QImage::Format_RGBA8888);
    glBindTexture( GL_TEXTURE_2D, texture);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, dimension, dimension, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.mirrored(false,true).bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

// grid.js
void GridWindow::initGridBuffersAndTextures(){
    initGridBuffers();
    initGridTextures();
    m_gridInitialized = true;
}

// grid.js
void GridWindow::initGridBuffers(){

    int index = -1;
    foreach(GridImage *grid, m_GridImages){

        index++;
        float rows = grid->m_imagegrid->rows();
        float cols = grid->m_imagegrid->cols();

        Q_ASSERT(rows > 0 && cols > 0);

        ////////////// Allocate memory

        size_t tileBufferSize = 10 * sizeof ( GLfloat );
        size_t tilePositionBufferSize = sizeof( GLuint ) * rows * cols;

        grid->m_tilePositionBufferGrid = ( GLuint * )malloc( tilePositionBufferSize );
        memset( grid->m_tilePositionBufferGrid, 0, tilePositionBufferSize );


        grid->m_tiles = ( GLfloat ** )malloc( sizeof ( GLfloat * ) * rows * cols );

        // Init tile grid
        for( float row = 0; row < rows; row++ ){
            for( float col = 0; col < cols; col++ ){

                float tRow = (rows - row)  - (rows / 2.0f);
                float tCol = ( col ) - (cols / 2.0f);
                if(cols / 2.0 == 0){
                    tCol += 0.5f;
                }else{
                    tCol += 1.0f;
                }

                GLfloat *tile = ( GLfloat* )malloc( tileBufferSize );

                tile[ 0 ] = tCol-1;
                tile[ 1 ] = tRow;
                tile[ 2 ] = tCol;
                tile[ 3 ] = tRow;
                tile[ 4 ] = tCol;
                tile[ 5 ] = tRow-1;
                tile[ 6 ] = tCol-1;
                tile[ 7 ] = tRow-1;
                tile[ 8 ] = tCol-1;
                tile[ 9 ] = tRow;

                qint64 tileIndex = _tileIndex( row, col, cols );
                grid->m_tiles[ tileIndex ] = tile;
            }
        }

        // init texture coords
        grid->m_textureCoords = ( GLfloat * )malloc( tileBufferSize );
        grid->m_textureCoords[ 0 ] = 0.0f;
        grid->m_textureCoords[ 1 ] = 1.0f;
        grid->m_textureCoords[ 2 ] = 1.0f;
        grid->m_textureCoords[ 3 ] = 1.0f;
        grid->m_textureCoords[ 4 ] = 1.0f;
        grid->m_textureCoords[ 5 ] = 0.0f;
        grid->m_textureCoords[ 6 ] = 0.0f;
        grid->m_textureCoords[ 7 ] = 0.0f;
        grid->m_textureCoords[ 8 ] = 0.0f;
        grid->m_textureCoords[ 9 ] = 1.0f;

        ///////// Bind gl buffers
        glGenBuffers( rows * cols, grid->m_tilePositionBufferGrid );
        for(int row = 0; row < rows; row++){
            for(int col = 0; col < cols; col++){
                qint64 tileIndex = _tileIndex(row, col, cols);
                glBindBuffer( GL_ARRAY_BUFFER, grid->m_tilePositionBufferGrid[ tileIndex ] );
                glBufferData( GL_ARRAY_BUFFER, tileBufferSize, grid->m_tiles[ tileIndex ], GL_STATIC_DRAW );
                glBindBuffer( GL_ARRAY_BUFFER, 0 );
            }
        }

        ////////// Square vertex buffer
        glGenBuffers( 1, &grid->m_squareVertexTextureCoordBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, grid->m_squareVertexTextureCoordBuffer );
        glBufferData( GL_ARRAY_BUFFER, tileBufferSize , grid->m_textureCoords, GL_STATIC_DRAW );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    glGenTextures(1, &m_hudTextTextTexture);
    glBindTexture(GL_TEXTURE_2D, m_hudTextTextTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenBuffers(1, &m_hudTextVbo);
}

// grid.js
void GridWindow::initGridTextures(){
    foreach( GridImage *grid, m_GridImages ){
        int rows = grid->m_imagegrid->rows();
        int cols = grid->m_imagegrid->cols();

        size_t textureGridSize = rows * cols * sizeof( GLuint );
        grid->m_tileTextureGrid = ( GLuint * )malloc( textureGridSize );
        memset( grid->m_tileTextureGrid, 0, textureGridSize );
        glGenTextures( rows * cols, grid->m_tileTextureGrid );

        grid->m_tileTextureGrid2 = ( GLuint * )malloc( textureGridSize );
        memset( grid->m_tileTextureGrid2, 0, textureGridSize );
        glGenTextures( rows * cols, grid->m_tileTextureGrid2 );

        grid->m_imagegrid->loadTiles();
    }
}


void GridWindow::drawOverlayMeasurements( int x, int y, float w, float h ){

    QMatrix4x4 p;
    p.setToIdentity();

    //    p.ortho(0,0,w,h,0.01,100.0);
    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    p.translate( 0, 0, -0.01 );

    m_pMatrix = p;

    qreal zratio = qAbs( m_zoom / m_rulerZ);
    zratio = 1.0 / zratio;


    /// Draw measurement overlay

    m_mvStack.push(m_mvMatrix);
    m_mvMatrix.setToIdentity();

    m_mvMatrix.translate(0,0,m_rulerZ);

    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
    m_mvMatrix.translate( m_transX * zratio, m_transY * zratio, 0.0);

    ImageGrid * grid = m_GridImages.first()->m_imagegrid;

    QPointF topLeft = sm2gl( QPointF( 0,0 ), grid );
    QPointF BottomRight = sm2gl( QPointF( grid->m_stretchwidth, grid->m_stretchheight ), grid);

    /////////// Draw yellow overlay square

    //    GLfloat vertices[] = {
    //        zratio * topLeft.x(),     zratio * topLeft.y(), 0.0,
    //        zratio * topLeft.x(),     zratio * BottomRight.y(), 0.0,
    //        zratio * BottomRight.x(), zratio * BottomRight.y(), 0.0,
    //        zratio * BottomRight.x(), zratio * topLeft.y(), 0.0,
    //    };

    //    GLfloat colors[] = {
    //        1.0, 1.0, 0.0, 0.2,
    //        1.0, 1.0, 0.0, 0.2,
    //        1.0, 1.0, 0.0, 0.2,
    //        1.0, 1.0, 0.0, 0.2
    //    };

    //    glVertexAttribPointer(m_measurementVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    //    glVertexAttribPointer(m_measurementColorAttribute,  4, GL_FLOAT, GL_FALSE, 0, colors);

    //    glEnableVertexAttribArray(0);
    //    glEnableVertexAttribArray(1);

    //    setMeasurementUniforms();

    //    glDrawArrays(GL_QUADS, 0, 4);

    //    glDisableVertexAttribArray(1);
    //    glDisableVertexAttribArray(0);

    m_mvMatrix = m_mvStack.pop();
}

void GridWindow::drawOverlay1( int x, int y, float w, float h ){
    qreal overlay1z = -14.8;

    glLineWidth(0.6);

    GLfloat cubeColors[] = {
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f,
        0.0f, 1.0f, 0.0f, 0.6f
    };

    GLfloat cubeVertices[] = {
        // Front face
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,

        // Back face
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
        1.0,  1.0, -1.0,
        1.0, -1.0, -1.0,

        // Top face
        -1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
        1.0,  1.0,  1.0,
        1.0,  1.0, -1.0,

        // Bottom face
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,

        // Right face
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        1.0,  1.0,  1.0,
        1.0, -1.0,  1.0,

        // Left face
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0
    };


    // Rhombic Dodecahedron: as quads
    GLfloat rdVertices[] = {

        // A
        0.0,  0.0,  3.0,
        -1.5,  1.5,  1.5,
        0.0, 3.0, 0.0,
        1.5, 1.5, 1.5,

        // B
        -3.0, 0.0, 0.0,
        -1.5, 1.5, 1.5,
        0.0, 0.0, 3.0,
        -1.5, -1.5, 1.5,

        // C
        -3.0, 0.0, 0.0,
        -1.5, -1.5, 1.5,
        0.0, -3.0, 0.0,
        -1.5, -1.5, -1.5,

        // D
        0.0, 0.0, 3.0,
        -1.5, -1.5, 1.5,
        0.0, -3.0, 0.0,
        1.5, -1.5, 1.5,

        // E
        0.0, 0.0, 3.0,
        1.5, 1.5, 1.5,
        3.0, 0.0, 0.0,
        1.5, -1.5, 1.5,

        // F
        3.0, 0.0, 0.0,
        1.5, -1.5, 1.5,
        0.0, -3.0, 0.0,
        1.5, -1.5 -1.5,

        // G
        0.0, 0.0, -3.0,
        -1.5, -1.5, -1.5,
        0.0, -3.0, 0.0,
        1.5, -1.5, -1.5,

        // H
        0.0, 0.0, -3.0,
        1.5, 1.5, -1.5,
        3.0, 0.0, 0.0,
        1.5, -1.5, -1.5,

        // I
        3.0, 0.0, 0.0,
        1.5, 1.5, 1.5,
        0.0, 3.0, 0.0,
        1.5, 1.5, -1.5,

        // J
        0.0,  0.0,  -3.0,
        -1.5,  1.5,  -1.5,
        0.0, 3.0, 0.0,
        1.5, 1.5, -1.5,

        // K
        -3.0, 0.0, 0.0,
        -1.5, 1.5, -1.5,
        0.0, 0.0, -3.0,
        -1.5, -1.5, -1.5,

        // L
        0.0, 3.0, 0.0,
        -1.5, 1.5, 1.5,
        -3.0, 0.0, 0.0,
        -1.5, 1.5, -1.5
    };


    QMatrix4x4 p;
    p.setToIdentity();

    //    p.ortho(0,0,w,h,0.1,100.0);
    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    p.translate( 0, 0, overlay1z );

    m_pMatrix = p;

    ////////       Circle that shows rotation
    if( m_rotz != m_settings.rotation ){
        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.setToIdentity();
        m_mvMatrix.translate(0.0f,0.0f,-8.0f);
        m_mvMatrix.rotate( r2d( m_rotz ) , 0.0, 0.0, 1.0 );
        int dots = 12;
        float radius = 0.25;
        GLfloat centerX = 0.0;
        GLfloat centerY = 0.0;

        GLfloat vertices[ (dots  * 3) + ( 6 ) ];
        float stepSize = ( (2.0 * pi) / dots );
        int i = 0;
        for (float d = 0; d <= ( 2.0* pi ) - stepSize ; d += stepSize ) {
            vertices[ i++ ] = ( sin( d ) * radius ) + centerX;
            vertices[ i++ ] = ( cos( d ) * radius ) + centerY;
            vertices[ i++ ] = 0.0f;
        }

        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = 0.0f;

        vertices[ i++ ] = 0.0f;
        vertices[ i++ ] = 0.5f;
        vertices[ i++ ] = 0.0f;


        GLfloat colors[dots * 4 + 8];
        int j = 0;
        for(i = 0; i < dots; i++){
            colors[ j++ ] = 0.0;// red
            if(i % 2) {
                colors[ j++ ] = 1.0;// green
                colors[ j++ ] = 0.0;// blue
            }else{
                colors[ j++ ] = 0.0;// green
                colors[ j++ ] = 1.0;// blue
            }

            colors[ j++ ] = 1.0;// alpha
        }
        colors[ j++ ] = 1.0;// r
        colors[ j++ ] = 0.0;// g
        colors[ j++ ] = 0.0;// b
        colors[ j++ ] = 1.0;// a

        colors[ j++ ] = 1.0;// r
        colors[ j++ ] = 0.0;// g
        colors[ j++ ] = 0.0;// b
        colors[ j++ ] = 1.0;// a

        glVertexAttribPointer( m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertices );
        glVertexAttribPointer( m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, colors );

        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

        setHudMatrixUniforms();

        glDrawArrays( GL_LINE_LOOP, 0, dots );
        glDrawArrays( GL_LINES, dots , 2 );

        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 0 );
        m_mvMatrix = m_mvStack.pop();
    }

    //////// Ruler
    if( false ){ // m_zoom != m_settings.zoom ){

        m_mvStack.push(m_mvMatrix);
        m_mvMatrix.setToIdentity();

        m_mvMatrix.translate(0,0,m_rulerZ);

        GLfloat vertices[] = {
            -0.5, 0.0, 0.0,
            0.5, 0.0, 0.0,
            0.0, -0.5, 0.0,
            0.0, 0.5, 0.0
        };

        GLfloat colors[] = {
            0.0, 1.0, 0.0, 0.7,
            0.0, 1.0, 0.0, 0.7,
            0.0, 1.0, 0.0, 0.7,
            0.0, 1.0, 0.0, 0.7
        };

        glVertexAttribPointer(m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, colors);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        setHudMatrixUniforms();
        glDrawArrays(GL_QUAD_STRIP, 0, 4);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        m_mvMatrix = m_mvStack.pop();
    }

    //////// Rhombic dodecahedron
    ///
    if( false ){
        m_mvStack.push(m_mvMatrix);

        m_mvMatrix.setToIdentity();

        m_mvMatrix.rotate(m_frame, 0, 1, 0);

        // Set up cube vertices

        glVertexAttribPointer(m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, rdVertices);
        glVertexAttribPointer(m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, rdColors);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        setHudMatrixUniforms();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDrawArrays(GL_QUADS, 0, 24);
        //        glDrawArrays(GL_QUADS, 44, 4);

        glDisable(GL_DEPTH_TEST);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        m_mvMatrix = m_mvStack.pop();
    }

    //////// Zoom cube
    if( false ) { //  m_zoom != m_settings.zoom ){

        m_mvStack.push(m_mvMatrix);

        m_mvMatrix.setToIdentity();

        // Set up cube vertices

        glVertexAttribPointer(m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);
        glVertexAttribPointer(m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, cubeColors);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        setHudMatrixUniforms();
        glDrawArrays(GL_LINE_LOOP, 0, 24);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        m_mvMatrix = m_mvStack.pop();
    }
}

void GridWindow::drawStencil( GridLayer *layer ){

    _enableStencil();

    ///// Set up stencil vertices from layer polygon
    GLfloat stencilVertices[ layer->stencilPolygon.count() * 2 ];
    int stencilVerticesCount = 0;
    foreach( QPointF point, layer->stencilPolygon ){
        QPointF pt = sm2gl( point, layer->gridImage->m_imagegrid );
        stencilVertices[ stencilVerticesCount++ ] = (float)pt.x();
        stencilVertices[ stencilVerticesCount++ ] = (float)pt.y();
    }


    QString dbg;
    int j = 0;
    for(int i = 0; i < stencilVerticesCount /2; i++){
        dbg.append(QString("(%1, %2)")
                .arg(stencilVertices[j++])
                .arg(stencilVertices[j++]));
    }
    qDebug() << __FUNCTION__ << dbg;

    /////  Draw stencil to stencil buffer
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( m_stencilVertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, stencilVertices );

    setStencilMatrixUniforms();
    glDrawArrays( GL_POLYGON, 0, stencilVerticesCount / 2 );
    glDisableVertexAttribArray( 0 );

    _disableStencil();
}

void GridWindow::drawGrid( GridLayer *layer ){

    GridImage * grid = layer->gridImage;


    ///////// Draw grid
    glBindBuffer( GL_ARRAY_BUFFER, grid->m_squareVertexTextureCoordBuffer );
    glVertexAttribPointer( m_sceneTextureCoordAttribute, 2 , GL_FLOAT, GL_FALSE, 0, 0 );

    int rows = grid->m_imagegrid->rows();
    int cols = grid->m_imagegrid->cols();
    for( int row = 0; row < rows; row++ ) {

        /////// Animated tiles ( Press A to toggle )
        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.rotate( r2d( m_rotx ), 1, 0, 0 );
        m_mvMatrix.rotate( r2d( m_roty ), 0, 1, 0 );
        m_mvMatrix.rotate( r2d( m_animateSquare ), 0.4, 0.1, 0.8 );

        for( int col = 0; col < cols; col++ ) {
            int tileIndex = _tileIndex( row, col, cols );

            GLfloat *tile = grid->m_tiles[tileIndex];



            int j = 0;
            for(int i = 0; i < 8; i++){
//                qDebug() << __FUNCTION__ << QPointF(tile[j++], tile[j++]);
            }

            QPolygonF poly;
            poly << gl2sm( QPointF( tile[ 0 ], tile[ 1 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 2 ], tile[ 3 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 4 ], tile[ 5 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 6 ], tile[ 7 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 0 ], tile[ 1 ] ), grid->m_imagegrid );

            if(
                    layer->stencilPolygon.containsPoint( poly.at(0), Qt::OddEvenFill ) &&
                    layer->stencilPolygon.containsPoint( poly.at(1), Qt::OddEvenFill ) &&
                    layer->stencilPolygon.containsPoint( poly.at(2), Qt::OddEvenFill ) &&
                    layer->stencilPolygon.containsPoint( poly.at(3), Qt::OddEvenFill ) ){
                continue;
            }

            //////// Tile matrix
            // For "A" animation
            if(row * col % 2 == 0){
                m_mvMatrix.rotate( r2d( m_animateEven ), 0.2, 0.4, 0.6 );
            }else{
                m_mvMatrix.rotate( r2d( m_animateOdd ),  0.3, 1.0, 0.2 );
            }

            ///////////  Draw Tile
            //  Tile Buffer
            GLuint tileBuffer = grid->m_tilePositionBufferGrid[ tileIndex ];
            glBindBuffer( GL_ARRAY_BUFFER, tileBuffer );

            glVertexAttribPointer( m_sceneVertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0 );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );

            GLuint tileTexture;
            tileTexture = grid->m_tileTextureGrid[ tileIndex ];

            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, tileTexture );
            glUniform1i( m_sceneSamplerUniform, 0 );

            glEnableVertexAttribArray( 0 );
            glEnableVertexAttribArray( 1 );

            setSceneMatrixUniforms();
            setSceneColorUniforms();

            glDrawArrays( GL_TRIANGLE_STRIP, 0, 5 );

            glDisableVertexAttribArray( 1 );
            glDisableVertexAttribArray( 0 );

            glBindBuffer( GL_ARRAY_BUFFER, 0);
            glBindTexture( GL_TEXTURE_2D, 0);
        }
        m_mvMatrix = m_mvStack.pop();
    }
}

// grid.js
void GridWindow::handleLoadedGridTexture(int index, int row, int col ){

    GridImage *grid = m_GridImages.at(index);

    ImageGrid *imageGrid = grid->m_imagegrid;

    ImageTile *tile = imageGrid->tile(row,col);
    handleLoadedTexture(grid, tile->image(),       grid->m_tileTextureGrid [ _tileIndex( row, col, imageGrid->cols() ) ], imageGrid->dimension() );
    handleLoadedTexture(grid, tile->lowresImage(), grid->m_tileTextureGrid2[ _tileIndex( row, col, imageGrid->cols() ) ], imageGrid->dimension() * ( 1.0 / 4.0 ));

    Q_ASSERT( grid->m_imagegrid->dimension() > 0 &&
              tile->image().width() == tile->image().height() &&
              tile->image().width() == grid->m_imagegrid->dimension() );
}

void GridWindow::drawOverlayText( int x, int y, float w, float h ){
    QMatrix4x4 p;
    p.setToIdentity();

    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    p.translate( 0, 0, -0.01 );

    m_pMatrix = p;

    m_mvStack.push( m_mvMatrix );

    m_mvMatrix.setToIdentity();
    m_mvMatrix.translate(0.0f,0.0f,-1.0f);

    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    m_hudTextColor[0] = 1.0;//r
    m_hudTextColor[1] = 0.0;//g
    m_hudTextColor[2] = 1.0;//b
    m_hudTextColor[3] = 1.0;//a

    float sx = 2.0 / ( w  * 1.0 );
    float sy = 2.0 / ( h * 1.0 );

    /* Set font size to 48 pixels, color to black */
    FT_Set_Pixel_Sizes(m_face, 0, 12);

    setHudTextUniforms();
    QString px = QString("%1px").arg(_dbgZoom());
    QString sfps = QString("FPS: %1 ").arg(fps());

    //    render_text(px.toLatin1(), 0.40, -0.4, sx, sy);
    render_text(sfps.toLatin1(), -0.20, -0.4, sx, sy);

    if(m_rotz != m_settings.rotation){
        QString rot = QString( "Rotation: %1" ).arg(r2d( m_rotz ));
        render_text(rot.toLatin1(),  0.1, 0.0, sx, sy);
    }

    glDisable(GL_BLEND);

    m_mvMatrix = m_mvStack.pop( );
}

////////////////////////////////////////////////////////////////

void GridWindow::controlAnimate() {
    QDateTime dt;
    qint64 timeNow = dt.currentMSecsSinceEpoch();

    if (lastTime != 0) {
        qint64 elapsed = timeNow - lastTime;


        int samples = 5;

        /// Calculate FPS
        m_frameTime.push_back(elapsed);
        if(m_frameTime.count() > samples){
            m_frameTime.pop_front();
        }

        qreal sum = 0;
        foreach(int val, m_frameTime){
            sum +=val;
        }

        qreal avgFrameTime = sum / m_frameTime.count();
        m_fps  = 1000.0 / avgFrameTime ;

        if(m_animateOn){
            m_animateSquare += (75 * elapsed) / (m_flipfreq * 500) ;
            m_animateEven += (75 * elapsed) / (m_flipfreq * 400) ;
            m_animateOdd += (75 * elapsed) / (m_flipfreq * 200) ;
        }else{
            if(m_animateSquare > 0){
                m_animateSquare -= (75 * elapsed) / (m_flipfreq * 50) ;
                if(m_animateSquare < 0){
                    m_animateSquare = 0;
                }
            }
            if(m_animateEven > 0){
                m_animateEven -= (75 * elapsed) / (m_flipfreq * 40) ;
                if(m_animateEven < 0){
                    m_animateEven = 0;
                }
            }
            if(m_animateOdd > 0){
                m_animateOdd -= (75 * elapsed) / (m_flipfreq * 20) ;
                if(m_animateOdd < 0){
                    m_animateOdd = 0;
                }
            }
        }

        foreach( GridImage *image, m_GridImages ){
            foreach( GridLayer *layer, image->m_gridLayers ){
                if( layer->m_zrotation != layer->zrotation ){
                    layer->m_zrotation = layer->zrotation;
                }
            }
        }

        if(m_brightness != m_settings.brightness){
            float step = (m_settings.brightness - m_brightness) / 4.0;
            m_brightness += step;
            if(m_brightness > maxBrightness){
                m_brightness = maxBrightness;
            }
            if(m_brightness < minBrightness){
                m_brightness = minBrightness;
            }
        }

        if(m_contrast != m_settings.contrast){
            float step = (m_settings.contrast - m_contrast) / 4.0;
            m_contrast += step;
            if(m_contrast > maxContrast){
                m_contrast = maxContrast;
            }
            if(m_contrast < minContrast){
                m_contrast = minContrast;
            }
        }

        if(m_gamma != m_settings.gamma){
            float step = (m_settings.gamma - m_gamma) / 4.0;
            m_gamma += step;
            if(m_gamma > maxGamma){
                m_gamma = maxGamma;
            }
            if(m_gamma < minGamma){
                m_gamma = minGamma;
            }
        }

        if(m_settings.zoom != m_zoom){
            float step = (m_settings.zoom - m_zoom) / (fps() / 7.0);
            m_zoom += step;
            if(fabs(step) < 0.0001){
                m_zoom = m_settings.zoom;
            }
            if(m_zoom > m_maxZ){
                m_zoom = m_settings.zoom = m_maxZ;
            }
            if(m_zoom < m_minZ){
                m_zoom = m_settings.zoom = m_minZ;
            }
        }

        if(m_settings.transX != m_transX ||
                m_settings.transX > m_transX
                || m_settings.transX < m_transX){
            float step = (m_settings.transX - m_transX) / (fps() / 7.0);
            m_transX += step;
        }

        if(m_settings.transY != m_transY
                || m_settings.transY > m_transY ){
            float step = (m_settings.transY - m_transY) / (fps() / 7.0);
            m_transY += step;
        }

        if(m_settings.flipH){
            // increase from 0 to pi
            m_rotx += (75 *elapsed) / m_flipfreq;
            if(m_rotx > pi ){
                m_rotx = pi ;
            }
        }else{
            // decrease from pi to 0
            m_rotx -= (75 * elapsed) / m_flipfreq;
            if(m_rotx < 0.0){
                m_rotx = 0.0;
            }
        }

        float flipMod = 1.0;
        if(m_vflip90 == true){
            flipMod = 0.5;
        }

        if(m_settings.flipV){
            // increase from 0 to pi
            m_roty += (75 * elapsed) / m_flipfreq ;
            if(m_roty > (pi * flipMod)  ){
                m_roty = (pi * flipMod) ;
            }
        }else{
            // decrease from pi to 0
            m_roty -= (75 * elapsed) / m_flipfreq ;
            if(m_roty < 0.0){
                m_roty = 0.0;
            }
        }

        float step = (m_settings.rotation - m_rotz) / 20.0;
        if(m_rotz < m_settings.rotation ){
            m_rotz += step;
            if(m_rotz > m_settings.rotation){
                // rotated too far
                m_rotz = m_settings.rotation;
            }
        }else{
            if( m_rotz > m_settings.rotation ){
                m_rotz += step;
                if(m_rotz < m_settings.rotation){
                    // rotated too far
                    m_rotz = m_settings.rotation;
                }
            }
        }
        if(fabs(step) < 0.0001){
            m_rotz = m_settings.rotation;
        }
    }
    lastTime = timeNow;
}


////////////////////////////////////////////////////////////////////

void GridWindow::toggleLayerDemo(){
    m_layerDemoOn = !m_layerDemoOn;
    qDebug() << "m_layerDemoOn:" << m_layerDemoOn;
}

void GridWindow::toggleWireframe(){
    m_wireframe = !m_wireframe;
    qDebug() << "WIREFRAME:" << m_wireframe;
}

void GridWindow::toggleOsteotomy(){
    m_osteotomyOn = !m_osteotomyOn;
    qDebug() << "OSTEOTOMY:" << m_osteotomyOn;
}

// control.js
void GridWindow::toggleAnimate(){
    m_animateOn = !m_animateOn;
    //    qDebug() << "ANIMATION:" << m_animateOn;
}

// control.js
void GridWindow::rotLeft90(){
    m_settings.rotation = pi / 2;
}

// control.js
void GridWindow::rotRight90(){
    m_settings.rotation = 3 *( pi / 2 );
}

// control.js
void GridWindow::setContrast ( qreal val ){
    m_settings.contrast = val;
    if(m_settings.contrast < minContrast){
        m_settings.contrast = minContrast;
    }
    if(m_settings.contrast > maxContrast){
        m_settings.contrast = maxContrast;
    }
    qDebug() << "CONTRAST:" << m_settings.contrast;
}

// control.js
void GridWindow::setBrightness( qreal val ){
    m_settings.brightness = val;
    if(m_settings.brightness < minBrightness){
        m_settings.brightness = minBrightness;
    }
    if(m_settings.brightness > maxBrightness){
        m_settings.brightness = maxBrightness;
    }
    qDebug() << "BRIGHTNESS:" << m_settings.brightness;
}

// control.js
void GridWindow::setGamma( qreal val ){
    m_settings.gamma = val;
    if(m_settings.gamma < minGamma){
        m_settings.gamma = minGamma;
    }
    if(m_settings.gamma > maxGamma){
        m_settings.gamma = maxGamma;
    }
    qDebug() << "GAMMA:" << m_settings.gamma;
}

// control.js
void GridWindow::mouseMoveEvent(QMouseEvent *event){
    QPoint pos = event->pos();

    int deltaX = pos.x() - m_lastMouse.x();
    int deltaY = pos.y() - m_lastMouse.y();

    Qt::MouseButtons buttonState = event->buttons();
    if (! ( buttonState & Qt::LeftButton ) ) {
        if( buttonState & Qt::RightButton){

            qreal dy = deltaY / 30.0;
            qreal dx = deltaX / 30.0;

            // Adjust BCG
            if(!isCommandKeyDown()){
                setBrightness (m_settings.brightness - dy);
                setContrast (m_settings.contrast + dx);
            }else{
                setGamma(m_settings.gamma - dx);
            }
        }
        return;
    }

    if(isCommandKeyDown() || isCtrlKeyDown()){
        if(deltaX > 0){
            for(int i = 0; i < deltaX; i++){
                panRight();
            }
        }else{
            for(int i = 0; i < -deltaX; i++){
                panLeft();
            }
        }

        if(deltaY > 0){
            for(int i = 0; i < deltaY; i++){
                panDown();
            }
        }else{
            for(int i = 0; i < -deltaY; i++){
                panUp();
            }
        }
    }

    m_lastMouse = pos;
}

// control.js
void GridWindow::mousePressEvent(QMouseEvent *e){
    m_lastMouse = e->pos();
}

// control.js
void GridWindow::mouseReleaseEvent(QMouseEvent *e){

}

// control.js
void GridWindow::keyPressEvent(QKeyEvent *e){
    int keycode = e->key();
    m_currentlyPressedKeys[keycode] = true;
    switch(keycode){

    case Qt::Key_W:
        toggleWireframe();
        break;
    case Qt::Key_L:
        toggleLayerDemo();
        break;
    case Qt::Key_O:
        toggleOsteotomy();
        return;
        break;
    case Qt::Key_A:
        toggleAnimate();
        return;
        break;
    case Qt::Key_V:
        flipV();
        return;
        break;
    case Qt::Key_H:
        flipH();
        return;
        break;
    case Qt::Key_I:
        invert();
        return;
        break;
    }
    handleKeys();
}

void GridWindow::render(){
    handleKeys();
    _render( 0 );
    controlAnimate();
}

void GridWindow::_render( qint64 frame )
{
    const qreal retinaScale = devicePixelRatio();
    int x = 0;
    int y = 0;
    int w = width() * retinaScale;
    int h = height() * retinaScale;

    if(m_wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glViewport(x, y, w, h);
    glStencilMask(0xFF);
    glClear( GL_COLOR_BUFFER_BIT );
    //    glFrontFace(GL_CW);
    //    glCullFace(GL_BACK);
    //    glEnable(GL_CULL_FACE);
    drawScene(x, y, w, h);
    drawMeasurements(x, y, w, h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawHud(x,y,w,h);
    m_frame++;
}

void GridWindow::drawScene( int x, int y, float w, float h ){

    Q_UNUSED(x)
    Q_UNUSED(y)

    _updateLayers();

    QMatrix4x4 p;
    p.setToIdentity();
    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    m_pMatrix = p;


    /// Scene Transform
    m_mvStack.push( m_mvMatrix );
    m_mvMatrix.setToIdentity();
    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
    m_mvMatrix.translate( m_transX, m_transY, m_zoom );
//    m_mvMatrix.rotate( grid->q.scalar(), grid->q.vector() );
    m_mvMatrix.rotate( m_sceneRotation );

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


    int gcount = 0;
    int lcount = 0;

    foreach( GridImage *grid, m_GridImages ){
        gcount++;
        foreach( GridLayer *layer, grid->m_gridLayers ){
            lcount++;
            //// Layer transform
            m_mvStack.push( m_mvMatrix );

            m_mvMatrix.translate( layer->translate );
            m_mvMatrix.rotate( layer->m_zrotation, 0, 0, 1 );

            m_stencilProgram->bind();
            drawStencil( layer );
            m_stencilProgram->release();

            m_sceneProgram->bind();
            drawGrid( layer );
            m_sceneProgram->release();

            glDisable( GL_STENCIL_TEST );
            glDisable( GL_DEPTH_TEST );

            m_mvMatrix = m_mvStack.pop();

        }
    }
//    qDebug() << __FUNCTION__ << "Grid:" << gcount << "Layers:" << lcount;
}

void GridWindow::_updateLayers(){
    ////////////// Osteotomy test demo
    if( m_osteotomyOn ){
        int testLayerCount = 4;
        /// two layers for first image only
        GridImage *grid = m_GridImages.first();
        if( grid ){
            if( grid->m_gridLayers.count() == testLayerCount ){
                //
            }else{
                /// create testLayerCount layers
                foreach( GridLayer *layer, grid->m_gridLayers ){
                    delete layer;
                }
                grid->m_gridLayers.clear();

                srand( QDateTime::currentMSecsSinceEpoch() );

                for(int i = 0; i < testLayerCount; i++){

                    GridLayer * layer = new GridLayer;

                    QPolygonF poly = QPolygonF();

                    int width = grid->m_imagegrid->m_image.width();
                    int height = grid->m_imagegrid->m_image.height();

                    QPointF pt1 = QPointF( width /  ((rand() % 10) * 1.0), 0);
                    QPointF pt2 = QPointF( width                          , height /  ((rand() % 10) * 1.0));
                    QPointF pt3 = QPointF( width /  ((rand() % 10) * 1.0), height);
                    QPointF pt4 = QPointF( 0                              , height /  ((rand() % 10) * 1.0));

                    poly << pt1 << pt2 << pt3 << pt4 << pt1;

                    layer->stencilPolygon = poly;


                    if( i < testLayerCount / 2 ) {
                        layer->translate = QVector3D(
                                    3.0 / ( rand() %3 ) * 1.0,
                                    testLayerCount % 3 ?
                                        3.0 / ( rand() %3 ) * 1.0 :
                                        -3.0 / ( rand() %3 ) * 1.0,
                                    0);
                        layer->zrotation = (rand() * 1.0);
                        layer->gridImage = grid;
                    }else{
                        layer->translate = QVector3D(
                                    -3.0 / (rand() %3 ) * 1.0,
                                    testLayerCount % 3 ?
                                        3.0 / ( rand() %3 ) * 1.0 :
                                        -3.0 / ( rand() %3 ) * 1.0,
                                    0);
                        layer->zrotation = (rand() * 1.0);
                        layer->gridImage = grid;
                    }
                    grid->m_gridLayers << layer;
                }
            }
        }else{
            Q_ASSERT(false);
        }
    }else{
        /// one layer
        GridImage *grid = m_GridImages.first();
        if(grid){
            if(grid->m_gridLayers.count() == 1){
                //
            }else{
                /// create 1 layer
                foreach(GridLayer *layer, grid->m_gridLayers){
                    delete layer;
                }
                grid->m_gridLayers.clear();

                GridLayer * layer = new GridLayer;

                QPolygonF poly = QPolygonF();

                int width = grid->m_imagegrid->m_image.width();
                int height = grid->m_imagegrid->m_image.height();

                QPointF pt1 = QPointF( width /  ((rand() % 10) * 1.0), 0);
                QPointF pt2 = QPointF( width                          , height /  ((rand() % 10) * 1.0));
                QPointF pt3 = QPointF( width /  ((rand() % 10) * 1.0), height);
                QPointF pt4 = QPointF( 0                              , height /  ((rand() % 10) * 1.0));

                poly << pt1 << pt2 << pt3 << pt4 << pt1;

                layer->stencilPolygon = poly;
                layer->translate = QVector3D(0,0,0);
                layer->zrotation = 0.0;
                layer->gridImage = grid;

                grid->m_gridLayers << layer;
            }
        }else{
            Q_ASSERT(false);
        }
    }
}

void GridWindow::drawHud( int x, int y, float w, float h ){
    m_hudProgram->bind();
    drawOverlay1( x, y, w, h );
    m_hudProgram->release();

    m_hudTextProgram->bind();
    drawOverlayText( x, y, w, h );
    m_hudTextProgram->release();
}

void GridWindow::drawMeasurements( int x, int y, int w, int h ){
    m_measurementsProgram->bind();
    drawOverlayMeasurements( x, y, w, h );
    m_measurementsProgram->release();
}

qreal GridWindow::fps(){
    return m_fps;
}

qreal GridWindow::_dbgZoom(){

    qreal dx = qAbs( m_zoom / m_rulerZ);
    qreal dim = m_GridImages.first()->m_imagegrid->dimension();

    return  dim * dx;
}

// controls.js
void GridWindow::zoomIn(){
    m_zoomStep = m_settings.zoom / 10.0f;
    m_settings.zoom += m_zoomStep;
    if ( m_settings.zoom >  m_maxZ ) {
        m_settings.zoom = m_maxZ;
    }
    //    _dbgZoom();
}

// controls.js
void GridWindow::zoomOut(){
    m_zoomStep = m_settings.zoom / 10.0f;
    m_settings.zoom -= m_zoomStep;
    if ( m_settings.zoom <  m_minZ ) {
        m_settings.zoom = m_minZ;
    }
    _dbgZoom();
}

// control.js
void GridWindow::panUp()   {
    qreal y = -m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN UP:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panDown() {
    qreal y = m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN DOWN:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panLeft() {
    qreal x = m_settings.zoom * m_panBase;
    qreal y = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN LEFT:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panRight() {
    qreal x = -m_settings.zoom * m_panBase;
    qreal y = 0.0;;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN RIGHT:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::rotateLeft() {
    m_settings.rotation -= pi / 180.0;
    //    qDebug() << "Rot Left" << m_settings.rotation ;
}

// control.js
void GridWindow::rotateRight() {
    m_settings.rotation += pi / 180.0;
    //    qDebug() << "Rot Right" << m_settings.rotation ;
}

// control.js
void GridWindow::invert() {
    m_settings.invert = !m_settings.invert;
    //    qDebug() << "Invert" << m_settings.invert ;
}

// control.js
void GridWindow::handleKeys() {
    if(m_currentlyPressedKeys[Qt::Key_R]) {       // RESET
        reset();
    }

    if(m_currentlyPressedKeys[Qt::Key_C]) {       // COLOR RESET
        resetColor();
    }

    if(m_currentlyPressedKeys[Qt::Key_T]){        // ORIENTATION RESET
        resetOrientation();
    }

    if(m_currentlyPressedKeys[Qt::Key_PageUp]) {  // ZOOM IN
        zoomIn();
    }
    if(m_currentlyPressedKeys[Qt::Key_PageDown]){ // ZOOM OUT
        zoomOut();
    }
    if(m_currentlyPressedKeys[Qt::Key_Right]){    // PAN LEFT
        panLeft();
    }
    if(m_currentlyPressedKeys[Qt::Key_Down]){     // PAN UP
        panUp();
    }
    if(m_currentlyPressedKeys[Qt::Key_Left]){     // PAN RIGHT
        panRight();
    }
    if(m_currentlyPressedKeys[Qt::Key_Up]){       // PAN DOWN
        panDown();
    }
    if(m_currentlyPressedKeys[Qt::Key_End]){      // ROT RIGHT
        rotateRight();
    }
    if(m_currentlyPressedKeys[Qt::Key_Home]){     // ROT LEFT
        rotateLeft();
    }
    if(isCommandKeyDown()){
        if(QApplication::mouseButtons() & Qt::LeftButton){
            setCursor(QCursor(Qt::ClosedHandCursor));
        }else{
            setCursor(QCursor(Qt::OpenHandCursor));
        }
    }else{
        unsetCursor();
    }
}

// control.js
void GridWindow::keyReleaseEvent(QKeyEvent *e){
    m_currentlyPressedKeys[e->key()] = false;
}

void GridWindow::reset(){
    resetColor();
    resetAnimation();
    resetOrientation();
}

void GridWindow::addImage( ImageGrid *imageGrid, QQuaternion q )
{
    GridImage *grid = new GridImage;
    grid->m_imagegrid = imageGrid;
    grid->q = q;

    GridLayer *defaultLayer = new GridLayer;
    defaultLayer->gridImage = grid;

    qreal width = imageGrid->m_image.width();
    qreal height = imageGrid->m_image.height();

    QPolygonF poly ;
    poly << QPointF(0,0)
         << QPointF(0, height)
         << QPointF(width,height)
         << QPointF(width, 0)
         << QPointF(0,0);

    defaultLayer->stencilPolygon = poly;

    grid->m_gridLayers.append( defaultLayer );

    m_GridImages.append( grid );

    int index = m_GridImages.indexOf( grid );
    imageGrid->setIndex( index );

    bool ok;
    ok = connect(grid->m_imagegrid, SIGNAL( tileImageLoaded( int,int,int )), SLOT( handleLoadedGridTexture( int,int,int ) ));Q_ASSERT(ok);
}

void GridWindow::setVFlip90(bool b)
{
    m_vflip90 = b;
}

void GridWindow::setSceneRotation( QQuaternion q )
{
    m_sceneRotation = q;
}

// controls.js
void GridWindow::resetColor(){
    m_settings.invert = false;
    m_settings.brightness = initBrightness;
    m_settings.contrast = initContrast;
    m_settings.gamma = initGamma;
}

void GridWindow::resetAnimation(){
    m_animateSquare = 0;
    m_animateEven = 0;
    m_animateOdd = 0;
    m_animateOn = false;
}

// controls.js
void GridWindow::resetOrientation(){
    m_settings.transY = 0.0;
    m_settings.transX = 0.0;

    m_settings.rotation = 0.0;
    m_settings.zoom = m_initZ;
    m_settings.flipH = false;
    m_settings.flipV = false;
}

// controls.js // Flip image over its X axis
void GridWindow::flipH() {
    m_settings.flipH = !m_settings.flipH;
    //    qDebug() << "FlipH" << m_settings.flipH;
}

// controls.js // Flip image over its Y axis
void GridWindow::flipV() {
    m_settings.flipV = !m_settings.flipV;
    //    qDebug() << "FlipV" << m_settings.flipV;
}

// controls.js // Flip over X axis relative to view
void GridWindow::flipX() {
    /// TODO:
}

// controls.js // Flip over X axis relative to view
void GridWindow::flipY() {
    /// TODO:
}

// controls.js
bool GridWindow::isCommandKeyDown()
{
    return
            m_currentlyPressedKeys[ Qt::Key_Alt ] ||
            m_currentlyPressedKeys[ Qt::Key_Control ];
}

// controls.js
bool GridWindow::isCtrlKeyDown()
{
    return m_currentlyPressedKeys[ Qt::Key_Control ];
}

// controls.js
void GridWindow::resizeEvent( QResizeEvent * )
{

}

// controls.js
void GridWindow::wheelEvent( QWheelEvent *e )
{
    float delta = e->delta() / 70.0;

    if( !m_currentlyPressedKeys[ Qt::Key_Shift ] && isCommandKeyDown() ){
        if(delta > 0){
            zoomIn();
        }
        if(delta < 0){
            zoomOut();
        }
    }
    if( m_currentlyPressedKeys[ Qt::Key_Shift ] && isCommandKeyDown() ){
        float amt = 0.0f;
        if( delta > 0 ){

            amt = 1.0 / 90.0f;
        }
        if( delta < 0 ){
            amt = -1.0 / 90.0f;
        }
        m_settings.rotation += ( amt ) ;
        //        qDebug() << "Rotation" << m_settings.rotation;
    }
    //    qDebug() << "WHEEL:" << delta;
}

void GridWindow::render_text(const char *text, float x, float y, float sx, float sy) {
    const char *p;
    FT_GlyphSlot g = m_face->glyph;

    /* Create a texture that will be used to hold one "glyph" */
    GLuint tex;

    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    glUniform1i( m_hudTextSamplerUniform, 0 );

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    /* Linear filtering usually looks best for text */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray( m_hudTextVertexPositionAttribute );
    glBindBuffer( GL_ARRAY_BUFFER, m_hudTextVbo );
    glVertexAttribPointer( m_hudTextVertexPositionAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    /* Loop through all characters */
    for ( p = text; *p; p++ ) {
        /* Try to load and render the character */
        if ( FT_Load_Char( m_face, *p, FT_LOAD_RENDER ))
            continue;

        /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
        glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer );

        /* Calculate the vertex and texture coordinates */
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        point box[4] = {
            {x2, -y2, 0, 0},
            {x2 + w, -y2, 1, 0},
            {x2, -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        /* Draw the character on the screen */
        glBufferData( GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

        /* Advance the cursor to the start of the next character */
        x += ( g->advance.x >> 6 ) * sx;
        y += ( g->advance.y >> 6 ) * sy;
    }

    glDisableVertexAttribArray( m_hudTextVertexPositionAttribute );
    glDeleteTextures( 1, &tex );
}
