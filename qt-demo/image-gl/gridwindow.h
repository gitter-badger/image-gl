#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include "openglwindow.h"
#include "openglcontextview.h"
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QStack>
#include <QMatrix4x4>
#include <QQueue>

class QGestureEvent;
class QPanGesture;
class QSwipeGesture;
class QPinchGesture;

#include <ft2build.h>
#include FT_FREETYPE_H

class ImageGrid;

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


class GridImage;
class GridLayer {
public:

    GridLayer( GridImage * );
    ~GridLayer();

    void setPolygon( QPolygonF &poly );
    QPolygonF stencilPolygon();
    GLfloat *stencilVertices();
    GridImage *gridImage();

    void setTransformationMatrix( QMatrix4x4 &m );
    QMatrix4x4 transformationMatrix();

    qreal zrotation;
    qreal m_zrotation;
    QVector3D translate;

private:
    GridImage *m_gridImage;
    QMatrix4x4 m_transformationMatrix;
    QPolygonF m_stencilPolygon;
    GLfloat *m_stencilVertices;
};


class GridWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GridWindow();
    virtual ~GridWindow();

    virtual void initialize() ;
    virtual void render();

    virtual void reset();

    void addImage(ImageGrid *grid , QQuaternion q = QQuaternion());
    void removeImage(ImageGrid *imageGrid);


    static qreal r2d( qreal r);
    static qreal d2r( qreal d );
    static QPointF sm2gl( QPointF pixPos, ImageGrid *grid );
    static QPointF gl2sm( QPointF glPos, ImageGrid *grid );
    qreal unitToPx( ImageGrid *grid );

    qreal fps();

    void deinitialize();

    static qint64 tileIndex(qint64 row, qint64 col, qint64 cols);

public slots:
    void panDelta(int x, int y);
    void fitToView();
    void setVFlip90( bool );
    void setSceneRotation( QQuaternion );

    virtual void handleLoadedGridTexture( int index, int row, int column );

    void setContrast( qreal val );
    void setBrightness( qreal val );
    void setGamma( qreal val );

    // control
    void resetOrientation();
    void resetAnimation();
    void resetColor();

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
    void rotRight90();
    void rotLeft90();
    void zoomIn();
    void zoomOut();
    void toggleAnimate();
    void toggleOsteotomy();
    void toggleLayerDemo();
    void toggleWireframe();

protected:
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture *pan);
    void swipeTriggered(QSwipeGesture *swipe);
    void pinchTriggered(QPinchGesture *pinch);

    virtual void drawScene(int x, int y, float w, float h);
    virtual void drawHud(int x, int y, float w, float h);
    virtual void drawMeasurements(int x, int y, int w, int h);

    void handleLoadedTexture(GridImage *grid, QImage image, GLuint texture, float dimension);


    void drawOverlayMeasurements( int, int, float, float );
    void drawOverlay1( int, int, float, float );
    void drawOverlayText( int x, int y, float w, float h );
    void drawStencil( GridLayer *layer );
    void drawGrid( GridLayer *layer );

    void render_text(const char *text, float x, float y, float sx, float sy);


private:

    void  _updateLayers();

    qreal _dbgZoom();
    void  _render(qint64 frame);
    void  _enableStencil();
    void  _disableStencil();

    void controlAnimate();
    bool isCtrlKeyDown();
    bool isCommandKeyDown();


    virtual bool nativeGestureEvent(QNativeGestureEvent *event);
//    void touchEvent(QTouchEvent *event);
//    void tabletEvent(QTabletEvent *event);
    void hideEvent(QHideEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


    void updateInvert();
    void updateBCG();

    void webGLStart();
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

    int   _initTextResources();
    FT_Library m_ft;
    FT_Face m_face;
    FT_GlyphSlot m_glyph;

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
};

#endif // GRIDWINDOW_H
