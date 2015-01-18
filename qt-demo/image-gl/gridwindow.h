#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <openglwindow.h>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QStack>

struct GLSettings {
    GLfloat zoom;
    GLboolean flipH;
    GLboolean flipV;
    GLboolean invert;
    GLfloat brightness;
    GLfloat contrast;
    GLfloat gamma;
    GLfloat rotation;

    GLfloat transX ;
    GLfloat transY ;
};

class ImageGrid;
class GridWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GridWindow();
    virtual ~GridWindow();

    virtual void initialize() ;
    virtual void deinitialize();
    virtual void render();

    void resetSettings();
    void reset();

    void animate();

    void setGrid(ImageGrid *grid);


    void setContrast(qreal val);
    void setBrightness(qreal val);
    void setGamma(qreal val);

public slots:
    void panUp();
    void panDown();
    void panLeft();
    void panRight();
    void rotateLeft();
    void rotateRight();
    void invert();
    void handleKeys();
    void flipH();
    void flipV();
    void flipX();
    void flipY();
    // control
    void toggleAnimate();
    void rotLeft90();
    void rotRight90();
    void zoomIn();
    void zoomOut();

    void handleLoadedGridTexture(int row, int column);


protected:
    void handleLoadedTexture(QImage image, GLuint texture);
    void drawScene(int x, int y, float w, float h);
    void drawGrid(int, int, float, float);
    void drawTriangle(int, int, float, float);


private:
    void _render();

    bool isCtrlKeyDown();
    bool isCommandKeyDown();

    void closeEvent();
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    qint64 _tileIndex(qint64 row, qint64 col);

    void updateInvert();
    void updateBCG();

    void webGLStart();
    void initShadersGrid();
    void initShadersTriangle();
    void initGridBuffersAndTextures();
    void initGridBuffers();
    void initGridTextures();

    void setMatrixUniforms();
    void setColorUniforms();
    GLuint loadShader(GLenum type, const char *source);

    GLint m_vertexPositionAttribute;
    GLint m_textureCoordAttribute;
    GLint m_colorAttribute;
    GLint m_uInvert;

    GLint m_pMatrixUniform;
    GLint m_mvMatrixUniform;
    GLint m_samplerUniform;

    bool m_gridInitialized;
    QOpenGLShaderProgram *m_program;
    int m_frame;

    QMatrix4x4 m_pMatrix;
    QMatrix4x4 m_mvMatrix;

    GLSettings m_settings;

    GLuint m_animateSquare;

    GLboolean m_animateOn = false;

    ImageGrid *m_imagegrid;

    GLuint m_squareVertexTextureCoordBuffer;

    GLuint *m_tilePositionBufferGrid;
    GLuint *m_tileTextureGrid;

    GLfloat **m_tiles;

    // Animation
    // Variables used for animating flips & rotation
    GLfloat m_flipfreq ;
    GLfloat m_rotx ;
    GLfloat m_roty ;
    GLfloat m_rotz ;
    qint64 lastTime ;

    GLfloat m_animateEven ;
    GLfloat m_animateOdd ;

    GLfloat m_panBase;

    GLfloat m_zoom;
    GLfloat m_transX;
    GLfloat m_transY;

    GLfloat *m_pColor;
    GLfloat *m_textureCoords;
    bool m_currentlyPressedKeys[Qt::Key_unknown];
    QPoint m_lastMouse;

    QStack<QMatrix4x4> m_mvStack;
};

#endif // GRIDWINDOW_H
