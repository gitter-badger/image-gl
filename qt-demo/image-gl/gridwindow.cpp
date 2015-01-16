#include "gridwindow.h"
#include "imagegrid.h"
#include "imagetile.h"
#include <QScreen>
#include <QDateTime>
#include <math.h>

void debug(QString from, QString doing) {
   GLenum error = glGetError();
   if(error != GL_NO_ERROR){
       qDebug() << from << doing << error;
   }
}

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

static const char *vertexShaderSourceG =
        "attribute vec3 aVertexPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "uniform mat4 uColor;\n"
        "varying vec2 vTextureCoord;\n"
        "void main(void) {\n"
          "gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);\n"
          "vTextureCoord = aTextureCoord;\n"
        "}\n";

//        "precision mediump float;\n"
static const char *fragmentShaderSourceG =
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
        "vColor.x = 1.0;\n" // for testing: this is to debug where there is some drawing vs nothing.
        "vColor.y = 0.0;\n"
        "vColor.z = 0.0;\n"
        "vColor.a = 1.0;\n"
           "gl_FragColor = vColor;\n"
        "}\n";

GLuint GridWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader( type );
    glShaderSource ( shader, 1, &source, 0 );
    debug(__FUNCTION__, QString("bglShaderSource"));

    glCompileShader ( shader );
    debug(__FUNCTION__, QString("glCompileShader"));

    return shader;
}

GridWindow::GridWindow()
    : m_program(0),
      m_frame(0),
      m_flipfreq(4000.0),
      m_rotx(0),
      m_roty(0),
      m_rotz(0),
      lastTime(0),
      m_panBase(0.03),
      m_animateEven(0),
      m_animateOdd(0)
{
    bool ok = connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    Q_ASSERT(ok);
    m_timer.setInterval(1000);
}

GridWindow::~GridWindow()
{
    qint64 count = m_imagegrid->rows() * m_imagegrid->cols();
    glDeleteBuffers( count, m_tilePositionBufferGrid);
    glDeleteBuffers( count, m_tileTextureGrid);

    glDeleteBuffers( 1, &m_bcgColorBuffer );
    glDeleteBuffers( 1, &m_squareVertexTextureCoordBuffer );

    free ( m_tilePositionBufferGrid );
    free ( m_tileTextureGrid );
}

void GridWindow::setGrid(ImageGrid *grid)
{
    m_imagegrid = grid;
}

qint64 GridWindow::_tileIndex(qint64 row, qint64 col){
    qint64 cols = m_imagegrid->cols();
    qint64 index = cols * row + col;
    return index;
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

    glClearColor(0.0, 0.0, 1.0, 1.0);            debug(__FUNCTION__, QString("glClearColor"));
    glEnable(GL_DEPTH_TEST);                     debug(__FUNCTION__, QString("glEnable(GL_DEPTH_TEST)"));

    m_timer.start();
}

// gl-bp.js
void GridWindow::initGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);    debug(__FUNCTION__,"glViewport");
}

void GridWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);    debug(__FUNCTION__,"glViewport");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                 debug(__FUNCTION__,"glClear");

    m_program->bind();
    drawScene(0, 0, width() * retinaScale, height() * retinaScale);
    m_program->release();
}

// gl-shader.js
void GridWindow::updateBCG( GLfloat brightness, GLfloat contrast, GLfloat gamma ) {
    glUniform4f(m_uBCG, brightness, contrast, gamma , 1.0);                                       debug( __FUNCTION__,"glUniform4f" );
}

// gl-shader.js
void GridWindow::updateInvert( bool invert ){
    if(invert){
        glUniform1i(m_uInvert, 1);
    }else{
        glUniform1i(m_uInvert, 0);
    }
    debug( __FUNCTION__, "glUniform1i" );
}

// gl-shader.js
void GridWindow::initShaders(){
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceG );
    m_program->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceG );
    m_program->link();

    m_vertexPositionAttribute       = m_program->attributeLocation( "aVertexPosition" ); debug(__FUNCTION__, QString("initShaders"));
    m_textureCoordAttribute         = m_program->attributeLocation( "aTextureCoord" );   debug(__FUNCTION__, QString("initShaders"));

    m_mvMatrixUniform               = m_program->uniformLocation( "uMVMatrix" ); debug(__FUNCTION__, QString("initShaders"));
    m_pMatrixUniform                = m_program->uniformLocation( "uPMatrix" );  debug(__FUNCTION__, QString("initShaders"));
    m_uBCG 				            = m_program->uniformLocation( "uBCG" );      debug(__FUNCTION__, QString("initShaders"));
    m_uInvert 			            = m_program->uniformLocation( "uInvert" );   debug(__FUNCTION__, QString("initShaders"));
    m_samplerUniform 	            = m_program->uniformLocation( "uSampler" );  debug(__FUNCTION__, QString("initShaders"));
    m_uColorUniform            		= m_program->uniformLocation( "uColor" );    debug(__FUNCTION__, QString("initShaders"));
}

// gl-bp.js
void GridWindow::setMatrixUniforms(){
    m_program->setUniformValue(m_pMatrixUniform, m_pMatrix);      debug(__FUNCTION__,"program: setUniformValue, pMatrix");
    m_program->setUniformValue(m_mvMatrixUniform, m_mvMatrix);    debug(__FUNCTION__,"program: setUniformValue, mvMatrix");
}

// gl-bp.js
void GridWindow::handleLoadedTexture(QImage image, GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);                                                        debug(__FUNCTION__,"glBindTexture");
//    glPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());    debug(__FUNCTION__,"glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                            debug(__FUNCTION__,"glTexParameteri");
    glBindTexture(GL_TEXTURE_2D, 0);                                                              debug(__FUNCTION__,"glBindTexture");
}

// grid.js
void GridWindow::initBuffersAndTextures(){
    initBuffers();
    initTextures();
}

// grid.js
void GridWindow::initBuffers(){
    int rows = m_imagegrid->rows();
    int cols = m_imagegrid->cols();

    Q_ASSERT(rows > 0 && cols > 0);

    size_t tileBufferSize = 10 * sizeof ( GLfloat );
    size_t colorBufferSize = 4 * sizeof( GLfloat );
    size_t tilePositionBufferSize = sizeof( GLuint ) * rows * cols;

    m_tilePositionBufferGrid = (GLuint *)malloc( tilePositionBufferSize );
    memset( m_tilePositionBufferGrid, 0, tilePositionBufferSize );

    glGenBuffers( rows * cols, m_tilePositionBufferGrid );                    debug(__FUNCTION__, QString("before loop: glGenBuffers"));



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

            GLfloat *tile = ( GLfloat* )malloc( tileBufferSize );

            tile[0] = tCol-1;
            tile[1] = tRow;
            tile[2] = tCol;
            tile[3] = tRow;
            tile[4] = tCol;
            tile[5] = tRow-1;
            tile[6] = tCol-1;
            tile[7] = tRow-1;
            tile[8] = tCol-1;
            tile[9] = tRow;

            qint64 tileIndex = _tileIndex(row, col);
            glBindBuffer( GL_ARRAY_BUFFER, m_tilePositionBufferGrid[ tileIndex ] );            debug(__FUNCTION__, QString("tile loop: glBindBuffer"));
            glBufferData( GL_ARRAY_BUFFER, tileBufferSize, tile, GL_STATIC_DRAW );                         debug(__FUNCTION__, QString("tile loop: glBufferData"));
        }
    }


    m_textureCoords = ( GLfloat * )malloc( tileBufferSize );
    m_textureCoords[0] = 0.0;
    m_textureCoords[1] = 1.0;
    m_textureCoords[2] = 1.0;
    m_textureCoords[3] = 1.0;
    m_textureCoords[4] = 1.0;
    m_textureCoords[5] = 0.0;
    m_textureCoords[6] = 0.0;
    m_textureCoords[7] = 0.0;
    m_textureCoords[8] = 0.0;
    m_textureCoords[9] = 1.0;

    glGenBuffers( 1, &m_squareVertexTextureCoordBuffer );                                        debug(__FUNCTION__, QString("after loop: glGenBuffers 2"));
    glBindBuffer( GL_ARRAY_BUFFER, m_squareVertexTextureCoordBuffer );                           debug(__FUNCTION__, QString("after loop: glBindBuffer 2"));
    glBufferData( GL_ARRAY_BUFFER, tileBufferSize , m_textureCoords, GL_STATIC_DRAW );    debug(__FUNCTION__, QString("after loop: glBufferData 1"));


    // Init color buffer
    m_puBCG = (GLfloat *) malloc( colorBufferSize );

    glGenBuffers( 1, &m_bcgColorBuffer );                                                        debug(__FUNCTION__, QString("after loop: glGenBuffers 2"));
    glBindBuffer( GL_ARRAY_BUFFER, m_bcgColorBuffer );                                           debug(__FUNCTION__, QString("after loop: glBindBuffer 2"));
    glBufferData( GL_ARRAY_BUFFER, colorBufferSize , m_puBCG, GL_STATIC_DRAW );             debug(__FUNCTION__, QString("after loop: glBufferData 2"));
}

// grid.js
void GridWindow::initTextures(){
    int rows = m_imagegrid->rows();
    int cols = m_imagegrid->cols();

    size_t size = rows * cols * sizeof( GLuint );
    m_tileTextureGrid = (GLuint *)malloc( size );
    memset( m_tileTextureGrid, 0, size );

    glGenBuffers(rows * cols, m_tileTextureGrid);                               debug(__FUNCTION__,"loop: setMatrixUniforms");

    m_imagegrid->loadTiles();
}

// grid.js
void GridWindow::drawScene(int x, int y, int w, int h){
    drawGrid( x, y, w, h );
}

// grid.js
void GridWindow::drawGrid(int x, int y, int w, int h){
    debug(__FUNCTION__, "beginning draw grid");

    m_pMatrix.perspective( 45.0f, w / h, 0.1f, 100.0f );

    m_mvMatrix.setToIdentity(); // reset matrix

    m_mvMatrix.rotate(QQuaternion(m_settings.rotation, 0,0,1));

    m_mvMatrix.translate(m_settings.transX, m_settings.transY, m_settings.zoom);

    m_mvStack.push(m_mvMatrix);
    m_mvMatrix.rotate(QQuaternion(m_rotx, 1, 0, 1));
    m_mvMatrix.rotate(QQuaternion(m_roty, 0, 1, 0));
    m_mvMatrix.rotate(QQuaternion(m_animateSquare, 0.4, 0.1, 0.8));

    glBindBuffer(GL_ARRAY_BUFFER, m_squareVertexTextureCoordBuffer);             debug(__FUNCTION__,"glBindBuffer: m_squareVertexTextureCoordBuffer");
    glVertexAttribPointer(m_textureCoordAttribute, 2 , GL_FLOAT, false, 0, 0);    debug(__FUNCTION__,"glVertexAttribPointer: m_textureCoordAttribute");

    glBindBuffer(GL_ARRAY_BUFFER, m_bcgColorBuffer);                             debug(__FUNCTION__,"glBindBuffer: m_bcgColorBuffer");
    glVertexAttribPointer(m_uBCG, 4, GL_FLOAT, false, 0, 0);                     debug(__FUNCTION__,"glVertexAttribPointer: m_uBCG");

    int rows = m_imagegrid->rows();
    int cols = m_imagegrid->cols();

    for( int row = 0; row < rows; row++ ) {
        for( int col = 0; col < cols; col++ ) {

            int tileIndex = _tileIndex( row, col );
            glEnable(GL_TEXTURE_2D);                                                           debug(__FUNCTION__,"loop: glEnable(GL_TEXTURE_2D)");

            glBindBuffer( GL_ARRAY_BUFFER, m_tilePositionBufferGrid[ tileIndex ] );            debug(__FUNCTION__,"loop: glBindBuffer: m_tilePositionBufferGrid");
            glVertexAttribPointer( m_vertexPositionAttribute, 2, GL_FLOAT, false, 0, 0);       debug(__FUNCTION__,"loop: glVertexAttribPointer: m_vertexPositionAttribute");

            glActiveTexture( GL_TEXTURE0 );                                                    debug(__FUNCTION__,"loop: glActiveTexture");
            glBindTexture( GL_TEXTURE_2D, m_tileTextureGrid[ tileIndex ] );                    debug(__FUNCTION__,"loop: glBindTexture");
            glUniform1i( m_samplerUniform, 0 );                                                debug(__FUNCTION__,"loop: glUniform1i");

            setMatrixUniforms();                                                               debug(__FUNCTION__,"loop: setMatrixUniforms");

            glEnableVertexAttribArray(0);                                                      debug(__FUNCTION__,"loop: glEnableVertexAttribArray0");
            glEnableVertexAttribArray(1);                                                      debug(__FUNCTION__,"loop: glEnableVertexAttribArray1");
            glEnableVertexAttribArray(2);                                                      debug(__FUNCTION__,"loop: glEnableVertexAttribArray2");

            updateBCG( m_settings.brightness, m_settings.contrast, m_settings.gamma );
            updateInvert(m_settings.invert);

            glDrawArrays( GL_TRIANGLE_STRIP, 0, 5);                                            debug(__FUNCTION__,"loop: glDrawArrays");

            glDisableVertexAttribArray(2);                                                     debug(__FUNCTION__,"loop: glDisableVertexAttribArray");
            glDisableVertexAttribArray(1);                                                     debug(__FUNCTION__,"loop: glDisableVertexAttribArray");
            glDisableVertexAttribArray(0);                                                     debug(__FUNCTION__,"loop: glDisableVertexAttribArray");

            glDisable(GL_TEXTURE_2D);                                                          debug(__FUNCTION__,"loop: glDisable(GL_TEXTURE_2D)");

            // For "A" animation
            if(row * col % 2 == 0){
                m_mvMatrix.rotate(QQuaternion(m_animateEven, 1.0, 0.4, 0.6));
            }else{
                m_mvMatrix.rotate(QQuaternion(m_animateOdd,  0.3, 1.0, 0.2));
            }
            debug(__FUNCTION__,"loop: glRotatef");
        }
    }
//    glPopMatrix();    debug(__FUNCTION__,"loop: glPopMatrix");
    m_mvMatrix = m_mvStack.pop();
}

// grid.js
void GridWindow::handleLoadedGridTexture( int row, int col ){
    ImageTile *tile = m_imagegrid->tile(row,col);
    handleLoadedTexture( tile->image(), m_tileTextureGrid[ _tileIndex( row, col ) ]);

    Q_ASSERT(m_imagegrid->dimension() > 0 &&
             tile->image().width() == tile->image().height() &&
             tile->image().width() == m_imagegrid->dimension());
}

////////////////////////////////////////////////////////////////

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

        if(m_settings.flipH){
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
        if(m_settings.flipV){
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

        if(m_rotz < m_settings.rotation){
            m_rotz += (75 * elapsed) / m_flipfreq;
            if(m_rotz > m_settings.rotation){
                // rotated too far
                m_rotz = m_settings.rotation;
            }
        }
        if(m_rotz > m_settings.rotation){
            m_rotz -= (75 * elapsed) / m_flipfreq;
            if(m_rotz > m_settings.rotation){
                // rotated too far
                m_rotz = m_settings.rotation;
            }
        }
    }
    lastTime = timeNow;
}


////////////////////////////////////////////////////////////////////

// control.js
void GridWindow::toggleAnimate(){
    m_animateOn = !m_animateOn;
    qDebug() << "ANIMATION:" << m_animateOn;
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

            qreal dy = deltaY / 10.0;
            qreal dx = deltaX / 10.0;

            // Adjust BCG
            if(isCommandKeyDown()){
                setBrightness (m_settings.brightness - dy);
                setContrast (m_settings.contrast - dx);
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
    switch(keycode){
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
    m_currentlyPressedKeys[keycode] = true;
    handleKeys();
}

void GridWindow::tick(){
    handleKeys();
    render();
    animate();
}

// controls.js
void GridWindow::zoomIn(){
    if (m_settings.zoom >=  m_maxZ) {
        m_settings.zoom = m_maxZ;
    }else{
        m_settings.zoom += 0.01;
    }
    qDebug() << "ZOOM in:" << m_settings.zoom;
}

// controls.js
void GridWindow::zoomOut(){
    if (m_settings.zoom <=  m_minZ) {
        m_settings.zoom = m_minZ;
    }else{
        m_settings.zoom -= 0.01;
    }
    qDebug() << "ZOOM out:" << m_settings.zoom;
}

// control.js
void GridWindow::panUp()   {
    qreal y = -m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos(m_settings.rotation) + y * sin(m_settings.rotation);
    m_settings.transY += y * cos(m_settings.rotation) - x * sin(m_settings.rotation);
    qDebug() << "PAN UP:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panDown() {
    qreal y = m_settings.zoom * m_panBase;
    qreal x = 0.0;
    m_settings.transX += x * cos(m_settings.rotation) + y * sin(m_settings.rotation);
    m_settings.transY += y * cos(m_settings.rotation) - x * sin(m_settings.rotation);
    qDebug() << "PAN DOWN:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panLeft() {
    qreal x = m_settings.zoom * m_panBase;
    qreal y = 0.0;
    m_settings.transX += x * cos(m_settings.rotation) + y * sin(m_settings.rotation);
    m_settings.transY += y * cos(m_settings.rotation) - x * sin(m_settings.rotation);
    qDebug() << "PAN LEFT:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::panRight() {
    qreal x = -m_settings.zoom * m_panBase;
    qreal y = 0.0;;
    m_settings.transX += x * cos(m_settings.rotation) + y * sin(m_settings.rotation);
    m_settings.transY += y * cos(m_settings.rotation) - x * sin(m_settings.rotation);
    qDebug() << "PAN RIGHT:" << m_settings.transX << m_settings.transY;
}

// control.js
void GridWindow::rotateLeft() {
    m_settings.rotation -= pi / 180.0;
    qDebug() << "Rot Left" << m_settings.rotation ;
}

// control.js
void GridWindow::rotateRight() {
    m_settings.rotation += pi / 180.0;
    qDebug() << "Rot Right" << m_settings.rotation ;
}

// control.js
void GridWindow::invert() {
    m_settings.invert = !m_settings.invert;
    qDebug() << "Invert" << m_settings.invert ;
}

// control.js
void GridWindow::handleKeys() {
    if(m_currentlyPressedKeys[Qt::Key_I]) { // i key
        invert();
    }
    if(m_currentlyPressedKeys[Qt::Key_R]) { // r key
        if(isCtrlKeyDown()){
            resetSettings();
        }else{
            reset();
        }
    }
    if(m_currentlyPressedKeys[Qt::Key_PageUp]) { // Page Up
        zoomIn();
    }
    if(m_currentlyPressedKeys[Qt::Key_PageDown]){ // Page Down
        zoomOut();
    }
    if(m_currentlyPressedKeys[Qt::Key_Left]){ // LEft
        panLeft();
    }
    if(m_currentlyPressedKeys[Qt::Key_Up]){ // UP
        panUp();
    }
    if(m_currentlyPressedKeys[Qt::Key_Right]){ // RIGHT
        panRight();
    }
    if(m_currentlyPressedKeys[Qt::Key_Down]){ // DOWN
        panDown();
    }
    if(m_currentlyPressedKeys[Qt::Key_End]){ // END
        rotateRight();
    }
    if(m_currentlyPressedKeys[Qt::Key_Home]){ // HOME
        rotateLeft();
    }
}

// control.js
void GridWindow::keyReleaseEvent(QKeyEvent *e){
    m_currentlyPressedKeys[e->key()] = false;
}


// controls.js
void GridWindow::resetSettings(){
    m_animateSquare = 0;
    m_animateEven = 0;
    m_animateOdd = 0;
    m_animateOn = false;

    m_settings.invert = false;
    m_settings.brightness = initBrightness;
    m_settings.contrast = initContrast;
    m_settings.gamma = initGamma;

    qDebug() << "settings reset";
}

// controls.js
void GridWindow::reset(){

    resetSettings();

    m_settings.transY = 0.0;
    m_settings.transX = 0.0;

    m_settings.rotation = 0.0;
    m_settings.zoom = m_initZ;
    m_settings.flipH = false;
    m_settings.flipV = false;
    qDebug() << "whole reset";
}

// controls.js // Flip image over its X axis
void GridWindow::flipH() {
    m_settings.flipH = !m_settings.flipH;
    qDebug() << "FlipH" << m_settings.flipH;
}

// controls.js // Flip image over its Y axis
void GridWindow::flipV() {
    m_settings.flipV = !m_settings.flipV;
    qDebug() << "FlipV" << m_settings.flipV;
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
    return m_currentlyPressedKeys[Qt::Key_Meta];
}

// controls.js
bool GridWindow::isCtrlKeyDown()
{
    return m_currentlyPressedKeys[Qt::Key_Control];
}

// controls.js
void GridWindow::resizeEvent(QResizeEvent *)
{

}

// controls.js
void GridWindow::wheelEvent(QWheelEvent *e)
{
    float delta = e->delta() / 120.0;

    if(isCommandKeyDown()){
        m_settings.zoom += delta / m_stepZ;
        if(m_settings.zoom > m_maxZ){
            m_settings.zoom = m_maxZ;
        }
        if(m_settings.zoom < m_minZ){
            m_settings.zoom = m_minZ;
        }
        qDebug() << "Zoom" << m_settings.zoom;
    }
    if(isCtrlKeyDown()){
        int amt = 0;
        if( delta > 0 ){
            amt = 1;
        }
        if( delta < 0 ){
            amt = -1;
        }
        amt = amt / ( 90 );
        m_settings.rotation += ( amt ) ;
        qDebug() << "Rotation" << m_settings.rotation;
    }
    qDebug() << "Wheel" << delta;
}
