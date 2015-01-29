#ifndef GRIDIMAGE_H
#define GRIDIMAGE_H

#include <QQuaternion>
#include <QOpenGLShaderProgram>

class ImageGrid;
class GridLayer;
class GridImage {

public:
    GridImage();
    ~GridImage();
    QList< GridLayer *> m_gridLayers;

    QQuaternion q; // Position of image
    ImageGrid *m_imagegrid;
    GLuint m_squareVertexTextureCoordBuffer;
    GLuint *m_tilePositionBufferGrid;
    GLuint *m_tileTextureGrid;
    GLuint *m_tileTextureGrid2;
    GLuint *m_tileTextureGrid3;
    GLfloat **m_tiles;
    GLfloat *m_textureCoords;


    bool m_initialized;
};


#endif // GRIDIMAGE_H
