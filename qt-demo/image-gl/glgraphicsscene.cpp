#include "glgraphicsscene.h"
#include <QPainter>
#include <QCoreApplication>
#include <QOpenGLContext>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QQueue>
#include <QStack>
#include <QDebug>
#include <QDateTime>
#include <QGLContext>

#include "model.h"
#include "gridimage.h"
#include "gridlayer.h"
#include "imagetile.h"
#include "graphics_util.h"

class GLGraphicsScenePrivate {
public:
    GLGraphicsScenePrivate(GLGraphicsScene *scene);

    void updateInvert();
    void updateBCG();

    QList<GridImage *> gridImages();

    void GLStart();
    void initShadersScene();
    void initShadersHud();
    void initShadersHudText();
    void initShaderMeasurements();
    void initShadersStencil();

    void initGridBuffersAndTextures();
    void initGridBuffers();
    void initGridTextures();

    void initColorBuffer();

    void setSceneMatrixUniforms();
    void setHudMatrixUniforms();
    void setHudTextUniforms();
    void setMeasurementUniforms();
    void setSceneColorUniforms();
    void setStencilMatrixUniforms();

    GLuint loadShader(GLenum type, const char *source);

    void _render(qint64 frame);
    void drawScene(int x, int y, float w, float h);
    void drawMeasurements(int x, int y, int w, int h);
    void drawHud(int x, int y, float w, float h);
    void _updateLayers();
    void enableStencil();
    void disableStencil();
    void drawOverlayMeasurements(int x, int y, float w, float h);
    void drawOverlay1(int x, int y, float w, float h);
    void drawStencil(GridLayer *layer);
    void drawGrid(GridLayer *layer);

    void handleLoadedTexture(GridImage *grid, QImage image, GLuint texture, float dimension);
    qreal fps();
    void fitToView();
    void zoomIn();
    void zoomOut();
    void panUp();
    void panDown();
    void panLeft();
    void panRight();
    void rotateLeft() {
        m_settings.rotation -= m_pi / 180.0;
        //    qDebug() << "Rot Left" << m_settings.rotation ;
    }
    void rotateRight();
    void invert();
    void _resetKeys();
    void reset();
    void removeImage(ImageGrid *imageGrid);
    void addImage(ImageGrid *imageGrid, QQuaternion q);
    void setVFlip90(bool b);
    void resetColor();
    void resetAnimation();
    void resetOrientation();
    void flipH();
    void flipV();
    void setSceneRotation(QQuaternion q);
private:
    GLGraphicsScene *m_Owner;

    /// Shader Programs
    QOpenGLShaderProgram *m_sceneProgram;
    QOpenGLShaderProgram *m_stencilProgram;
    QOpenGLShaderProgram *m_hudProgram;
    QOpenGLShaderProgram *m_hudTextProgram;
    QOpenGLShaderProgram *m_measurementsProgram;

    /// SCENE
    GLint m_sceneVertexPositionAttribute;
    GLint m_sceneTextureCoordAttribute;
    GLint m_sceneColorAttribute;
    GLint m_sceneUInvert;
    GLint m_sceneBCGUniform;
    GLint m_sceneMVMatrixUniform;
    GLint m_scenePMatrixUniform;
    GLint m_sceneSamplerUniform;

    /// STENCIL
    GLint m_stencilVertexPositionAttribute;
    GLint m_stencilMVMatrixUniform;
    GLint m_stencilPMatrixUniform;

    /// HUD
    GLint m_hudVertexPositionAttribute ;
    GLint m_hudColorAttribute;
    GLint m_hudPMatrixUniform;
    GLint m_hudMVMatrixUniform;

    /// HUD TEXT
    GLint m_hudTextVertexPositionAttribute;
    GLint m_hudTextColorUniform;
    GLint m_hudTextPMatrixUniform;
    GLint m_hudTextMVMatrixUniform;
    GLint m_hudTextSamplerUniform;
    GLuint m_hudTextTextTexture;
    GLuint m_hudTextVbo;
    QVector4D m_hudTextColor;

#ifdef HAS_FREETYPE2
    int   _initTextResources();
    FT_Library m_ft;
    FT_Face m_face;
    FT_GlyphSlot m_glyph;
#endif

    /// MEAUREMENT
    GLint m_measurementPMatrixUniform;
    GLint m_measurementMVMatrixUniform;
    GLint m_measurementVertexPositionAttribute;
    GLint m_measurementColorAttribute;
    QList< GridImage * > m_GridImages;

    bool m_gridInitialized;

    GLSettings m_settings;

    // Animation
    // Variables used for animating flips & rotation
    GLfloat m_flipfreq ;
    GLfloat m_rotx ;
    GLfloat m_roty ;
    GLfloat m_rotz ;
    qint64 lastTime ;
    GLfloat m_animateEven ;
    GLfloat m_animateOdd ;
    GLuint m_animateSquare;
    GLboolean m_animateOn = false;


    GLfloat m_panBase;
    GLfloat m_zoom;
    GLboolean m_osteotomyOn = false;
    GLboolean m_layerDemoOn = false;
    GLboolean m_wireframe = false;
    GLfloat m_transX = 0.0f;
    GLfloat m_transY = 0.0f;
    GLfloat m_centerX = 0.0f;
    GLfloat m_centerY = 0.0f;

    bool m_initialized = true;
    Model *m_model1;

    bool m_currentlyPressedKeys[Qt::Key_unknown]; // currently pressed keys
    QPoint m_lastMouse; // Last mouse recorded position

    QMatrix4x4 m_pMatrix; // Current perspective matrix
    QMatrix4x4 m_mvMatrix; // Current ModelView matrix

    QStack<QMatrix4x4> m_mvStack; // ModelView matrix stack

    GLfloat m_brightness;
    GLfloat m_contrast;
    GLfloat m_gamma;

    GLfloat m_zoomStep; // The calculated amount of zoom for the next zoom operation

    QQuaternion m_sceneRotation; // Used to set an rotation on the MV matrix

    // The field of View
    GLfloat m_fov;

    // This is used for a 90 degree rotate over the Y axis. In one of the Demo's
    // I showed how the 2 images can both be present in the scene concurrently but
    // at 90 degree angle from eachother- for example: AP and Lateral images
    bool m_vflip90;

    QQueue<qreal> m_frameTime; // Used to calculate FPS
    qreal m_fps; // Set to the calculated FPS
    int m_frame; // Frame counter

    void delRdColors();
    void initRdColors();
    GLfloat *rdColors = NULL;
    const GLfloat m_rulerZ = -20.0;
};

GLGraphicsScenePrivate::GLGraphicsScenePrivate(GLGraphicsScene *owner):
    m_Owner(owner),
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
    m_brightness(m_initBrightness),
    m_contrast(m_initContrast),
    m_gamma(m_initGamma),
    m_animateOn( false ),
    m_osteotomyOn( false ),
    m_layerDemoOn( false ),
    m_wireframe( false ),
    m_transX( 0.0f ),
    m_transY( 0.0f ),
    m_centerX( 0.0f ),
    m_centerY( 0.0f ),
    m_initialized( false )
{
    m_mvMatrix = QMatrix4x4();
    m_pMatrix  = QMatrix4x4();
}

void GLGraphicsScenePrivate::GLStart() {

    initShadersScene();
    initShadersHud();
    initShadersHudText();
    initShaderMeasurements();
    initShadersStencil();
    initGridBuffersAndTextures();

//    m_model1 = new Model( ":/objects/toyplane", 0.001 );
//    m_model1 = new Model( "/Users/Jon/Downloads/vrml/4021722_prt.obj", 0.001 );


    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    m_initialized = true;
}

void GLGraphicsScenePrivate::initShadersScene(){

    m_sceneProgram = new QOpenGLShaderProgram( m_Owner );

    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceG );
    qDebug() << __FUNCTION__ << "Vertex shader ok";Q_ASSERT(m_sceneProgram);

    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceG );
    qDebug() << __FUNCTION__ << "Fragment shader ok";Q_ASSERT(m_sceneProgram);

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

void GLGraphicsScenePrivate::initShadersHudText(){

    m_hudTextProgram = new QOpenGLShaderProgram( m_Owner );
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceT );
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceT );
    m_hudTextProgram->link();

    m_hudTextVertexPositionAttribute       = m_hudTextProgram->attributeLocation( "aVertexPosition" );

    m_hudTextColorUniform    			   = m_hudTextProgram->uniformLocation( "uColor" );
    m_hudTextPMatrixUniform                = m_hudTextProgram->uniformLocation( "uPMatrix" );
    m_hudTextMVMatrixUniform               = m_hudTextProgram->uniformLocation( "uMVMatrix" );
    m_hudTextSamplerUniform 	           = m_hudTextProgram->uniformLocation( "uSampler" );
}

void GLGraphicsScenePrivate::initShadersHud(){

    m_hudProgram = new QOpenGLShaderProgram( m_Owner );
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceH );
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceH );
    m_hudProgram->link();

    m_hudVertexPositionAttribute           = m_hudProgram->attributeLocation( "aVertexPosition" );
    m_hudColorAttribute 			       = m_hudProgram->attributeLocation( "aColor" );

    m_hudPMatrixUniform                    = m_hudProgram->uniformLocation( "uPMatrix" );
    m_hudMVMatrixUniform                   = m_hudProgram->uniformLocation( "uMVMatrix" );
}

void GLGraphicsScenePrivate::initShaderMeasurements(){
    m_measurementsProgram = new QOpenGLShaderProgram( m_Owner );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceM );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceM );
    m_measurementsProgram->link();

    m_measurementVertexPositionAttribute   = m_measurementsProgram->attributeLocation( "aVertexPosition" );
    m_measurementColorAttribute 	       = m_measurementsProgram->attributeLocation( "aColor" );

    m_measurementPMatrixUniform            = m_measurementsProgram->uniformLocation( "uPMatrix" );
    m_measurementMVMatrixUniform           = m_measurementsProgram->uniformLocation( "uMVMatrix" );
}


void GLGraphicsScenePrivate::initShadersStencil(){
    m_stencilProgram = new QOpenGLShaderProgram( m_Owner );
    m_stencilProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceS );
    m_stencilProgram->link();

    m_stencilVertexPositionAttribute      = m_stencilProgram->attributeLocation( "aVertexPosition" );

    m_stencilPMatrixUniform               = m_stencilProgram->uniformLocation( "uPMatrix" );
    m_stencilMVMatrixUniform              = m_stencilProgram->uniformLocation( "uMVMatrix" );
}

void GLGraphicsScenePrivate::setStencilMatrixUniforms(){
    m_stencilProgram->setUniformValue( m_stencilPMatrixUniform, m_pMatrix );
    m_stencilProgram->setUniformValue( m_stencilMVMatrixUniform, m_mvMatrix );
}

void GLGraphicsScenePrivate::setSceneMatrixUniforms(){
    m_sceneProgram->setUniformValue( m_scenePMatrixUniform, m_pMatrix );
    m_sceneProgram->setUniformValue( m_sceneMVMatrixUniform, m_mvMatrix );
}

void GLGraphicsScenePrivate::setHudMatrixUniforms(){
    m_hudProgram->setUniformValue( m_hudPMatrixUniform, m_pMatrix );
    m_hudProgram->setUniformValue( m_hudMVMatrixUniform, m_mvMatrix );
}

void GLGraphicsScenePrivate::setHudTextUniforms(){
    m_hudTextProgram->setUniformValue( m_hudTextPMatrixUniform,  m_pMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextMVMatrixUniform, m_mvMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextColorUniform,    m_hudTextColor);
}

void GLGraphicsScenePrivate::setMeasurementUniforms(){
    m_measurementsProgram->setUniformValue( m_measurementPMatrixUniform,  m_pMatrix );
    m_measurementsProgram->setUniformValue( m_measurementMVMatrixUniform, m_mvMatrix );
}

void GLGraphicsScenePrivate::setSceneColorUniforms(){
    updateBCG();
    updateInvert();
}

void GLGraphicsScenePrivate::updateBCG() {
    glUniform4f( m_sceneBCGUniform, m_brightness, m_contrast, m_gamma, 1.0 );
}

QList<GridImage *> GLGraphicsScenePrivate::gridImages()
{
    return m_GridImages;
}

void GLGraphicsScenePrivate::updateInvert() {
    glUniform1i( m_sceneUInvert, m_settings.invert == true ? 1 : 0 );
}

void GLGraphicsScenePrivate::enableStencil(){
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glDepthMask( GL_FALSE );
    glStencilFunc( GL_NEVER, 1, 0xFF );
    glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );  // draw 1s on test fail (always)

    // draw stencil pattern
    glStencilMask( 0xFF);
    glClear( GL_STENCIL_BUFFER_BIT );  // needs mask=0xFF
    glEnable( GL_STENCIL_TEST );
}

void GLGraphicsScenePrivate::disableStencil(){
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glDepthMask( GL_TRUE );
    glStencilMask( 0x00 );
    // draw where stencil's value is 0
    glStencilFunc( GL_EQUAL, 0, 0xFF );
    // draw only where stencil's value is 1
    glStencilFunc( GL_EQUAL, 1, 0xFF );
}

// grid.js
void GLGraphicsScenePrivate::initGridBuffersAndTextures(){
    initGridBuffers( );
    initGridTextures( );
    m_gridInitialized = true;
}

// grid.js
void GLGraphicsScenePrivate::initGridBuffers(){

    int index = -1;
    foreach( GridImage *grid, m_GridImages ){

        Q_ASSERT( !grid->m_initialized );

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

                qint64 indy = tileIndex( row, col, cols );
                grid->m_tiles[ indy ] = tile;
            }
        }

        //////// init texture coords
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
        for( int row = 0; row < rows; row++ ){
            for( int col = 0; col < cols; col++ ){
                qint64 indy = tileIndex( row, col, cols );
                glBindBuffer( GL_ARRAY_BUFFER, grid->m_tilePositionBufferGrid[ indy ] );
                glBufferData( GL_ARRAY_BUFFER, tileBufferSize, grid->m_tiles[ indy ], GL_STATIC_DRAW );
                glBindBuffer( GL_ARRAY_BUFFER, 0 );
            }
        }

        ////////// Square vertex buffer
        glGenBuffers( 1, &grid->m_squareVertexTextureCoordBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, grid->m_squareVertexTextureCoordBuffer );
        glBufferData( GL_ARRAY_BUFFER, tileBufferSize , grid->m_textureCoords, GL_STATIC_DRAW );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        grid->m_initialized = true;
    }

//    glGenTextures( 1, &m_hudTextTextTexture );
//    glBindTexture( GL_TEXTURE_2D, m_hudTextTextTexture );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
//    glBindTexture( GL_TEXTURE_2D, 0 );
//    glGenBuffers( 1, &m_hudTextVbo );
}

void GLGraphicsScenePrivate::initGridTextures(){
    foreach( GridImage *grid, m_GridImages ){
        int rows = grid->m_imagegrid->rows();
        int cols = grid->m_imagegrid->cols();

        size_t textureGridSize = rows * cols * sizeof( GLuint );
        grid->m_tileTextureGrid = ( GLuint * )malloc( textureGridSize );
        memset( grid->m_tileTextureGrid, 0, textureGridSize );
        glGenTextures( rows * cols, grid->m_tileTextureGrid );

        grid->m_imagegrid->loadTiles();
    }
}


void GLGraphicsScenePrivate::drawOverlayMeasurements( int x, int y, float w, float h ){

    QMatrix4x4 p;
    p.setToIdentity();

    //    p.ortho(0,0,w,h,0.01,100.0);
    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    p.translate( 0, 0, -0.01f );

    m_pMatrix = p;

    qreal zratio = qAbs( m_zoom / m_rulerZ);
    zratio = 1.0 / zratio;


//    /// Draw measurement overlay
//    m_mvStack.push( m_mvMatrix );
//    m_mvMatrix.setToIdentity();
//    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
//    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );

//    m_mvMatrix.translate( m_transX - m_centerX, m_transY - m_centerY, m_zoom );

    ////////  Model
    if( false ) {

//        m_mvStack.push( m_mvMatrix );
//        m_mvMatrix.setToIdentity();
//        m_mvMatrix.translate( 0, 0, 0 );
////        m_mvMatrix.rotate( m_frame, 0, 1, 0 );
//        m_mvMatrix.rotate( r2d( m_rotz ), 0, 0, 1 );



        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.setToIdentity();
        m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
        m_mvMatrix.translate( m_transX - m_centerX, m_transY - m_centerY, m_zoom );
    //    m_mvMatrix.rotate( grid->q.scalar(), grid->q.vector() );
        m_mvMatrix.rotate( m_sceneRotation );



        setMeasurementUniforms();
        m_model1->setScale( zratio * 0.0001 );
        m_model1->render( true, false );

        m_mvMatrix = m_mvStack.pop();
    }

//    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
//    m_mvMatrix.translate( m_transX * zratio, m_transY * zratio, 0.0);

//    ImageGrid * grid = m_GridImages.first()->m_imagegrid;

//    QPointF topLeft = sm2gl( QPointF( 0,0 ), grid );
//    QPointF BottomRight = sm2gl( QPointF( grid->m_stretchwidth, grid->m_stretchheight ), grid);

//    /////////// Draw yellow overlay square

//        GLfloat vertices[] = {
//            zratio * topLeft.x(),     zratio * topLeft.y(), 0.0,
//            zratio * topLeft.x(),     zratio * BottomRight.y(), 0.0,
//            zratio * BottomRight.x(), zratio * BottomRight.y(), 0.0,
//            zratio * BottomRight.x(), zratio * topLeft.y(), 0.0,
//        };

//        GLfloat colors[] = {
//            1.0, 1.0, 0.0, 0.2,
//            1.0, 1.0, 0.0, 0.2,
//            1.0, 1.0, 0.0, 0.2,
//            1.0, 1.0, 0.0, 0.2
//        };

//        glVertexAttribPointer(m_measurementVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertices);
////        glVertexAttribPointer(m_measurementColorAttribute,  4, GL_FLOAT, GL_FALSE, 0, colors);

//        glEnableVertexAttribArray(0);
////        glEnableVertexAttribArray(1);

//        setMeasurementUniforms();

//        glDrawArrays(GL_QUADS, 0, 4);

////        glDisableVertexAttribArray(1);
//        glDisableVertexAttribArray(0);

//    m_mvMatrix = m_mvStack.pop();
}

void GLGraphicsScenePrivate::drawOverlay1( int x, int y, float w, float h ){
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
        1.5, -1.5, -1.5,

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
         0.0,  0.0, -3.0,
        -1.5,  1.5, -1.5,
         0.0,  3.0,  0.0,
         1.5,  1.5, -1.5,

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

//    glEnable( GL_BLEND );
//    glEnable( GL_DEPTH_TEST );
//    glDepthFunc( GL_LEQUAL );
//    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//    glDisable( GL_BLEND );
//    glDisable( GL_DEPTH_TEST );

    ////////       Circle that shows rotation
    if( m_rotz != m_settings.rotation ){
        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.setToIdentity();
        m_mvMatrix.translate(0.0f,0.0f,-8.0f);
        m_mvMatrix.rotate( r2d( m_rotz ) , 0.0, 0.0, 1.0 );
        const int dots = 12;
        float radius = 0.25;
        GLfloat centerX = 0.0;
        GLfloat centerY = 0.0;

        GLfloat vertices[ (dots  * 3) + ( 6 ) ];
        float stepSize = ( (2.0 * m_pi) / dots );
        int i = 0;
        for (float d = 0; d <= ( 2.0* m_pi ) - stepSize ; d += stepSize ) {
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

        glVertexAttribPointer( m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertices );
        glVertexAttribPointer( m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, colors );

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        setHudMatrixUniforms();
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 0 );

        m_mvMatrix = m_mvStack.pop();
    }

    //////// Rhombic dodecahedron
    ///
    if( true ){
        m_mvStack.push( m_mvMatrix );

        m_mvMatrix.setToIdentity();

        m_mvMatrix.translate( -20, 20, -50 );
        m_mvMatrix.rotate( m_frame *4, 0, 1, 0 );

        // Set up cube vertices

        glVertexAttribPointer( m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, rdVertices );
        glVertexAttribPointer( m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, rdColors );

        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

        setHudMatrixUniforms();

//        glEnable( GL_DEPTH_TEST );
//        glDepthFunc( GL_LEQUAL );

        glDrawArrays( GL_TRIANGLE_STRIP, 0, 48 );

        glDisable( GL_DEPTH_TEST );

        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 0 );

        m_mvMatrix = m_mvStack.pop();
    }

    //////// Zoom cube
    if( false ) { //  m_zoom != m_settings.zoom ){

        m_mvStack.push(m_mvMatrix);

        m_mvMatrix.setToIdentity();

        m_mvMatrix.rotate( r2d( m_rotz ), 0.0, 0.0, 1.0 );

        m_mvMatrix.translate( 0, 0, 8 );
        m_mvMatrix.translate( m_transX, m_transY, m_zoom );

        // Set up cube vertices

        glVertexAttribPointer( m_hudVertexPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices );
        glVertexAttribPointer( m_hudColorAttribute,          4, GL_FLOAT, GL_FALSE, 0, cubeColors );

        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

        setHudMatrixUniforms();
        glDrawArrays( GL_LINE_LOOP, 0, 24 );

        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 0);

        m_mvMatrix = m_mvStack.pop();
    }
}

void GLGraphicsScenePrivate::drawStencil( GridLayer *layer ){

    enableStencil();

    /////  Draw stencil to stencil buffer
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( m_stencilVertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, layer->stencilVertices() );

    setStencilMatrixUniforms();
    glDrawArrays( GL_TRIANGLE_STRIP, 0, layer->stencilPolygon().count() );
    glDisableVertexAttribArray( 0 );

    disableStencil();
}

void GLGraphicsScenePrivate::drawGrid( GridLayer *layer ){

    GridImage * grid = layer->gridImage();

    ///////// Draw grid
    glBindBuffer( GL_ARRAY_BUFFER, grid->m_squareVertexTextureCoordBuffer );
    glVertexAttribPointer( m_sceneTextureCoordAttribute, 2 , GL_FLOAT, GL_FALSE, 0, 0 );

    int rows = grid->m_imagegrid->rows();
    int cols = grid->m_imagegrid->cols();
    for( int row = 0; row < rows; row++ ) {

        /////// Animated tiles ( Press A to toggle )
        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.rotate( r2d( m_animateSquare ), 0.4, 0.1, 0.8 );

        for( int col = 0; col < cols; col++ ) {
            int indy = tileIndex( row, col, cols );

            GLfloat *tile = grid->m_tiles[ indy ];

            int j = 0;
            for(int i = 0; i < 8; i++){
//                qDebug() << __FUNCTION__ << QPointF(tile[j++], tile[j++]);
            }

            //////// Don't draw the grid tile that is excluded from the stencil
            QPolygonF poly;
            poly << gl2sm( QPointF( tile[ 0 ], tile[ 1 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 2 ], tile[ 3 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 4 ], tile[ 5 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 6 ], tile[ 7 ] ), grid->m_imagegrid );
            poly << gl2sm( QPointF( tile[ 0 ], tile[ 1 ] ), grid->m_imagegrid );

            QPolygonF stencilPoly = layer->stencilPolygon();
            if(
                    !stencilPoly.containsPoint( poly.at(0), Qt::OddEvenFill ) &&
                    !stencilPoly.containsPoint( poly.at(1), Qt::OddEvenFill ) &&
                    !stencilPoly.containsPoint( poly.at(2), Qt::OddEvenFill ) &&
                    !stencilPoly.containsPoint( poly.at(3), Qt::OddEvenFill ) ){
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
            GLuint tileBuffer;
            tileBuffer = grid->m_tilePositionBufferGrid[ indy ];

            glBindBuffer( GL_ARRAY_BUFFER, tileBuffer );

            glVertexAttribPointer( m_sceneVertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0 );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );


            glActiveTexture( GL_TEXTURE0 );

//            GLuint tileTexture;
//            tileTexture = grid->m_tileTextureGrid[ indy ];
//            glBindTexture( GL_TEXTURE_2D, indy );

            /// Using QOpenGLTexture
            grid->m_tileTextureGridQt.at( indy )->bind();

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

void GLGraphicsScene::handleLoadedGridTexture(int index, int row, int col ){

    GridImage *grid = d->gridImages().at(index);
    ImageGrid *imageGrid = grid->m_imagegrid;
    ImageTile *tile = imageGrid->tile( row,col );

//    handleLoadedTesxture( grid, tile->image(), grid->m_tileTextureGrid [ tileIndex( row, col, imageGrid->cols() ) ], imageGrid->dimension() );

    /// Using QOpenGLTexture
    QOpenGLTexture *tex = new QOpenGLTexture( tile->image().convertToFormat( QImage::Format_RGBA8888 ).mirrored( false, true ) );
    tex->setMinificationFilter( QOpenGLTexture::Nearest );
    tex->setMagnificationFilter( QOpenGLTexture::Nearest );
    tex->setWrapMode( QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge );
    tex->setWrapMode( QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge );
    grid->m_tileTextureGridQt.insert( tileIndex( row, col, imageGrid->cols() ), tex );

    Q_ASSERT( grid->m_imagegrid->dimension() > 0 &&
              tile->image().width() == tile->image().height() &&
              tile->image().width() == grid->m_imagegrid->dimension() );
}

void GLGraphicsScenePrivate::handleLoadedTexture( GridImage *grid, QImage image, GLuint texture, float dimension ){

    Q_UNUSED( grid );

    QImage img = image.convertToFormat( QImage::Format_RGBA8888 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, dimension, dimension, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.mirrored( false, true ).bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture( GL_TEXTURE_2D, 0 );
}
#ifdef HAS_FREETYPE2
void GLGraphicsScenePrivate::drawOverlayText( int x, int y, float w, float h ){
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_hudTextColor[0] = 1.0;//r
    m_hudTextColor[1] = 0.0;//g
    m_hudTextColor[2] = 1.0;//b
    m_hudTextColor[3] = 1.0;//a

    float sx = 2.0 / ( w  * 1.0 );
    float sy = 2.0 / ( h * 1.0 );

    /// Android build is not configured for freetype
    /* Set font size to 48 pixels, color to black */
    FT_Set_Pixel_Sizes(m_face, 0, 12);

    setHudTextUniforms();
    QString px = QString("%1px").arg(_dbgZoom());
    QString sfps = QString("FPS: %1 ").arg(fps());
    QString title = QString("%1").arg(this->m_GridImages.first()->m_imagegrid->fileName());

    //    render_text(px.toLatin1(), 0.40, -0.4, sx, sy);
    render_text(sfps.toLatin1(), -0.20, 0.35, sx, sy);

    render_text(title.toLatin1(), 0.20, 0.35, sx, sy);

    if(m_rotz != m_settings.rotation){
        QString rot = QString( "Rotation: %1" ).arg(r2d( m_rotz ));
        render_text(rot.toLatin1(),  0.1, 0.0, sx, sy);
    }
    glDisable(GL_BLEND);

    m_mvMatrix = m_mvStack.pop( );
}
#endif

void GLGraphicsScenePrivate::_render( qint64 frame )
{

    if(m_GridImages.count() == 0)
        return;

    const qreal retinaScale = 1.0;
    int x = 0;
    int y = 0;
    int w = m_Owner->width() * retinaScale;
    int h = m_Owner->height() * retinaScale;

#ifndef Q_OS_ANDROID
    if(m_wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
#endif
    glViewport(x, y, w, h);
    glStencilMask(0xFF);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    drawScene(x, y, w, h);
    drawMeasurements(x, y, w, h);

#ifndef Q_OS_ANDROID
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    drawHud(x,y,w,h);
    m_frame++;
}

void GLGraphicsScenePrivate::drawScene( int x, int y, float w, float h ){

    Q_UNUSED(x)
    Q_UNUSED(y)

    /// TEST layers
    _updateLayers();

    QMatrix4x4 p;
    p.setToIdentity();
    p.perspective( m_fov,  w / h, 0.01f, 100.0f );
    m_pMatrix = p;


    /// Scene Transform
    m_mvStack.push( m_mvMatrix );
    m_mvMatrix.setToIdentity();
    m_mvMatrix.rotate( r2d(m_rotz), 0, 0, 1 );
    m_mvMatrix.translate( m_transX - m_centerX, m_transY - m_centerY, m_zoom );
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

        GridLayer *layer = grid->m_gridLayers.first();

        m_mvStack.push( m_mvMatrix );
        m_mvMatrix.rotate( r2d( m_rotx ), 1, 0, 0 );
        m_mvMatrix.rotate( r2d( m_roty ), 0, 1, 0 );
        m_mvMatrix.translate( layer->translate );
        m_mvMatrix.rotate( layer->m_zrotation, 0, 0, 1 );

        if(m_osteotomyOn){
            m_stencilProgram->bind();
            drawStencil( layer );
            m_stencilProgram->release();
        }

        m_sceneProgram->bind();
        drawGrid( layer );
        m_sceneProgram->release();

        glDisable( GL_STENCIL_TEST );
        glDisable( GL_DEPTH_TEST );

        m_mvMatrix = m_mvStack.pop();

        if(grid->m_gridLayers.count() == 1){
            continue;
        }

        foreach( GridLayer *layer, grid->m_gridLayers ){
            lcount++;
            //// Layer transform
            m_mvStack.push( m_mvMatrix );

            m_mvMatrix.translate( layer->translate );
            m_mvMatrix.rotate( layer->m_zrotation, 0, 0, 1 );

            if(m_osteotomyOn){
                m_stencilProgram->bind();
                drawStencil( layer );
                m_stencilProgram->release();
            }

            m_sceneProgram->bind();
            drawGrid( layer );
            m_sceneProgram->release();

            glDisable( GL_STENCIL_TEST );
            glDisable( GL_DEPTH_TEST );

            m_mvMatrix = m_mvStack.pop();
        }
    }
}

void GLGraphicsScenePrivate::_updateLayers(){
    ////////////// Osteotomy test demo
    if( m_osteotomyOn ){
        int testLayerCount = 10;
        /// two layers for first image only
        GridImage *gridImage = m_GridImages.first();
        if( gridImage ){
            if( gridImage->m_gridLayers.count() == testLayerCount ){
                //
            }else{
                /// create testLayerCount layers
                foreach( GridLayer *layer, gridImage->m_gridLayers ){
                    delete layer;
                }
                gridImage->m_gridLayers.clear();

                srand( QDateTime::currentMSecsSinceEpoch() );

                for(int i = 0; i < testLayerCount; i++){

                    GridLayer * layer = new GridLayer( gridImage );

                    QPolygonF poly = QPolygonF();

                    int width = gridImage->m_imagegrid->image().width();
                    int height = gridImage->m_imagegrid->image().height();

                    QPointF pt1 = QPointF( width /  ((rand() % 10) * 1.0), 0);
                    QPointF pt2 = QPointF( width                          , height /  ((rand() % 10) * 1.0));
                    QPointF pt3 = QPointF( width /  ((rand() % 10) * 1.0), height);
                    QPointF pt4 = QPointF( 0                              , height /  ((rand() % 10) * 1.0));

                    poly << pt1 << pt2 << pt3 << pt4 << pt1;

                    layer->setPolygon( poly );

                    if( i < testLayerCount / 2 ) {
                        layer->translate = QVector3D(
                                    3.0 / ( rand() % 16 ) * 1.0,
                                    testLayerCount % 2 ?
                                        3.0 / ( rand() % 16 ) * 1.0 :
                                        -3.0 / ( rand() % 16 ) * 1.0,
                                    0);
                        layer->zrotation = (rand()%360 * 1.0);

                    }else{
                        layer->translate = QVector3D(
                                    -3.0 / (rand() %6 ) * 1.0,
                                    testLayerCount % 2 ?
                                        3.0 / ( rand() % 16 ) * 1.0 :
                                        -3.0 / ( rand() % 16 ) * 1.0,
                                    0);
                        layer->zrotation = (rand() % 360 * 1.0);
                    }

                    gridImage->m_gridLayers << layer;
                }
            }
        }else{
            Q_ASSERT(false);
        }
    }else{
        /// one layer
        GridImage *gridImage = m_GridImages.first();
        if( gridImage ){
            if( gridImage->m_gridLayers.count() == 1 ){
                //
            }else{
                /// create 1 layer
                foreach( GridLayer *layer, gridImage->m_gridLayers ){
                    delete layer;
                }
                gridImage->m_gridLayers.clear();

                GridLayer * layer = new GridLayer( gridImage );

                QPolygonF poly = QPolygonF();

                int width = gridImage->m_imagegrid->image().width();
                int height = gridImage->m_imagegrid->image().height();

                QPointF pt1 = QPointF( -width, -height );
                QPointF pt2 = QPointF( width * 2, -height );
                QPointF pt3 = QPointF( width * 2, height * 2 );
                QPointF pt4 = QPointF( -width, height * 2 );

                poly << pt1 << pt2 << pt3 << pt4 << pt1;

                layer->setPolygon( poly );

                layer->translate = QVector3D(0,0,0);
                layer->zrotation = 0.0;

                gridImage->m_gridLayers << layer;
            }
        }else{
            Q_ASSERT(false);
        }
    }
}

void GLGraphicsScenePrivate::drawHud( int x, int y, float w, float h ){
    m_hudProgram->bind();
    drawOverlay1( x, y, w, h );
    m_hudProgram->release();

//#ifndef Q_OS_ANDROID
#ifdef HAS_FREETYPE2
//    m_hudTextProgram->bind();
//    drawOverlayText( x, y, w, h );
//    m_hudTextProgram->release();
#endif // HAS_FREETYPE2
//#endif // Q_OS_ANDROIDs
}

void GLGraphicsScenePrivate::drawMeasurements( int x, int y, int w, int h ){
    m_measurementsProgram->bind();
    drawOverlayMeasurements( x, y, w, h );
    m_measurementsProgram->release();
}

qreal GLGraphicsScenePrivate::fps(){
    return m_fps;
}

// Uses first grid image to fit
void GLGraphicsScenePrivate::fitToView()
{
    if(this->m_GridImages.count() > 0){
        if(GridImage *grid = this->m_GridImages.first()){
            if(ImageGrid *image = grid->m_imagegrid){
                int cols = image->cols();
                int rows = image->rows();


                float imgAspect = cols / rows;

                float wAspect = 1.0;
                if( m_Owner->width() == 0 || m_Owner->height() == 0 ){
                    wAspect = 1920.0 / 1080.0;
                }else{
                    wAspect = ( m_Owner->width() / m_Owner->height() );
                }

                m_settings.transX = 0.0;
                m_settings.transY = 0.0;

                int ctrl = imgAspect >= wAspect ? cols : rows;

                m_zoom = m_settings.zoom  = - 2 * (tan( m_fov / 2.0 ) * ( ctrl ));
            }
        }
    }
}

void GLGraphicsScenePrivate::zoomIn(){
    m_zoomStep = m_settings.zoom / 20.0f;
    m_settings.zoom += m_zoomStep;
    if ( m_settings.zoom >  m_maxZ ) {
        m_settings.zoom = m_maxZ;
    }
}

void GLGraphicsScenePrivate::zoomOut(){
    m_zoomStep = m_settings.zoom / 20.0f;
    m_settings.zoom -= m_zoomStep;
    if ( m_settings.zoom <  m_minZ ) {
        m_settings.zoom = m_minZ;
    }
}

void GLGraphicsScenePrivate::panUp()   {
    qreal y = -m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN UP:" << m_settings.transX << m_settings.transY;
}

void GLGraphicsScenePrivate::panDown() {
    qreal y = m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN DOWN:" << m_settings.transX << m_settings.transY;
}

void GLGraphicsScenePrivate::panLeft() {
    qreal x = m_settings.zoom * m_panBase;
    qreal y = 0.0;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN LEFT:" << m_settings.transX << m_settings.transY;
}

void GLGraphicsScenePrivate::panRight() {
    qreal x = -m_settings.zoom * m_panBase;
    qreal y = 0.0;;
    m_settings.transX += x * cos( m_settings.rotation ) + y * sin( m_settings.rotation );
    m_settings.transY += y * cos( m_settings.rotation ) - x * sin( m_settings.rotation );
    //    qDebug() << "PAN RIGHT:" << m_settings.transX << m_settings.transY;
}



void GLGraphicsScenePrivate::rotateRight() {
    m_settings.rotation += m_pi / 180.0;
    //    qDebug() << "Rot Right" << m_settings.rotation ;
}

void GLGraphicsScenePrivate::invert() {
    m_settings.invert = !m_settings.invert;
    //    qDebug() << "Invert" << m_settings.invert ;
}

void GLGraphicsScenePrivate::_resetKeys(){
    for(int i = 0; i < Qt::Key_unknown; i++){
        m_currentlyPressedKeys[i] = false;
    }
}

void GLGraphicsScenePrivate::reset(){
    _resetKeys();
    resetColor();
    resetAnimation();
    resetOrientation();
}

void GLGraphicsScenePrivate::removeImage( ImageGrid *imageGrid ){
    foreach( GridImage *g, m_GridImages ){
        if( g->m_imagegrid == imageGrid ){

            delete g;
            m_GridImages.removeOne( g );
            break;
        }
    }
}

void GLGraphicsScenePrivate::addImage( ImageGrid *imageGrid, QQuaternion q )
{
    GridImage *gridImage = new GridImage;
    gridImage->m_imagegrid = imageGrid;
    gridImage->q = q;

    GridLayer *defaultLayer = new GridLayer( gridImage );

    qreal width = imageGrid->image().width();
    qreal height = imageGrid->image().height();

    QPolygonF poly ;
    poly << QPointF( 0, 0 )
         << QPointF( 0, height )
         << QPointF( width, height )
         << QPointF( width, 0 )
         << QPointF( 0, 0 );

    defaultLayer->setPolygon( poly );


    float rows = gridImage->m_imagegrid->rows();
    float cols = gridImage->m_imagegrid->cols();

    QPointF test0 = sm2gl(poly.at(0),imageGrid);
    QPointF test2 = sm2gl(poly.at(2),imageGrid);

    Q_ASSERT(test0.x() == -(cols / 2.0));
    Q_ASSERT(test0.y() ==  (rows / 2.0));

//    gridImage->m_gridLayers.append( defaultLayer );

    if(m_GridImages.count() == 0){
        /// This is the first image, so center on its dimension
        m_centerX = -( ( gridImage->m_imagegrid->m_stretchwidth  - gridImage->m_imagegrid->image().width() )  / gridImage->m_imagegrid->dimension() ) / 2.0;
        m_centerY = +( ( gridImage->m_imagegrid->m_stretchheight - gridImage->m_imagegrid->image().height() ) / gridImage->m_imagegrid->dimension() ) / 2.0 ;
//        qDebug() << __FUNCTION__ << m_centerX << m_centerY;
    }

    m_GridImages.append( gridImage );

    int index = m_GridImages.indexOf( gridImage );
    imageGrid->setIndex( index );

    bool ok = false;
    Q_ASSERT( gridImage->m_imagegrid );
    Q_ASSERT( m_Owner );
    ok = QObject::connect( gridImage->m_imagegrid, SIGNAL( tileImageLoaded( int, int, int ) ), m_Owner, SLOT( handleLoadedGridTexture( int, int, int ) ) ); Q_ASSERT( ok );
}

void GLGraphicsScenePrivate::setVFlip90(bool b)
{
    m_vflip90 = b;
}

void GLGraphicsScenePrivate::setSceneRotation( QQuaternion q )
{
    m_sceneRotation = q;
}

void GLGraphicsScenePrivate::resetColor(){
    m_settings.invert = false;
    m_settings.brightness = m_initBrightness;
    m_settings.contrast = m_initContrast;
    m_settings.gamma = m_initGamma;
}

void GLGraphicsScenePrivate::resetAnimation(){
    m_animateSquare = 0;
    m_animateEven = 0;
    m_animateOdd = 0;
    m_animateOn = false;
}

void GLGraphicsScenePrivate::resetOrientation(){
    m_settings.transY = 0.0;
    m_settings.transX = 0.0;

    m_settings.rotation = 0.0;
    m_settings.zoom = m_initZ;
    m_settings.flipH = false;
    m_settings.flipV = false;

    fitToView();
}

// controls.js // Flip image over its X axis
void GLGraphicsScenePrivate::flipH() {
    m_settings.flipH = !m_settings.flipH;
    //    qDebug() << "FlipH" << m_settings.flipH;
}

// controls.js // Flip image over its Y axis
void GLGraphicsScenePrivate::flipV() {
    m_settings.flipV = !m_settings.flipV;
    //    qDebug() << "FlipV" << m_settings.flipV;
}





















GLGraphicsScene::GLGraphicsScene():
    d(new GLGraphicsScenePrivate(this)),
    m_initialized(false)
{

}

GLGraphicsScene::~GLGraphicsScene()
{
    delete d;
}

void GLGraphicsScene::initialize()
{
    d->GLStart();
    m_initialized = true;
}

void GLGraphicsScene::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool GLGraphicsScene::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QGraphicsScene::event(event);
    }
}

void GLGraphicsScene::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

//    if (isExposed())
        renderNow();
}

void GLGraphicsScene::renderNow()
{
//    if (!isExposed())
//        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
//        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

//    m_context->makeCurrent(this);

    if (needsInitialize) {
//        initializeOpenGLFunctions();
        initialize();
    }

//    render();

//    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void GLGraphicsScene::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void GLGraphicsScene::setImageGrid(ImageGrid *grid)
{
    d->addImage( grid, QQuaternion() );
}

void GLGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(!m_initialized)
        return;

    if(painter->paintEngine()->type() != QPaintEngine::OpenGL &&
            painter->paintEngine()->type() != QPaintEngine::OpenGL2){
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    d->_render(0);
}
