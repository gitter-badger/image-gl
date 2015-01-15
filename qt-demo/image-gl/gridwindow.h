#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <openglwindow.h>
#include <QOpenGLShaderProgram>

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
public:
    GridWindow();
    ~GridWindow();

    void initialize() ;
    void render() ;

    void resetSettings();
    void reset();

    void animate();

    void setGrid(ImageGrid *grid);

private slots:
    void handleLoadedGridTexture(int row, int column);
    void initTextures(qint64 tileCount);

protected:
    void handleLoadedTexture(QImage image, GLuint texture);
    void drawScene();

private:

    qint64 _tileIndex(qint64 row, qint64 col);

    void updateInvert(bool invert);
    void updateBCG(GLfloat brightness, GLfloat contrast, GLfloat gamma);

    void webGLStart();
    void initShaders();
    void initGL();
    void initBuffersAndTextures();
    void initBuffers();
    void initTextures();
    void drawGrid(int, int, int, int);

    void setMatrixUniforms();
    GLuint loadShader(GLenum type, const char *source);

    GLuint vertexPositionAttribute;
    GLuint textureCoordAttribute;
    GLuint uBCG;
    GLuint uInvert;

    GLuint pMatrixUniform;
    GLuint mvMatrixUniform;
    GLuint samplerUniform;


    GLuint colorUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    QMatrix4x4 pMatrix;
    QMatrix4x4 mvMatrix;

    GLSettings settings;

    GLuint *m_uTileTextures;

    GLuint m_animateSquare;

    GLboolean m_animateOn = false;

    ImageGrid *m_imagegrid;

    void *puBCG;
    GLuint bcgColorBuffer;
    GLuint squareVertexCoordBuffer;
    GLuint squareVertexTextureCoordBuffer;

    QList< GLuint > tilePositionBufferGrid;
    QList< GLuint > tileTextureGrid;

    GLuint gridBuffer(qint64 row, qint64 col);
    GLuint gridTexture(qint64 row, qint64 col);


    // Animation
    // Variables used for animating flips & rotation
    GLfloat m_flipfreq ;
    GLfloat m_rotx ;
    GLfloat m_roty ;
    GLfloat m_rotz ;
    qint64 lastTime ;

    GLfloat m_animateEven ;
    GLfloat m_animateOdd ;
};

#endif // GRIDWINDOW_H
