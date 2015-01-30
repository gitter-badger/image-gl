#ifndef GRIDIMAGE_H
#define GRIDIMAGE_H

#include <QQuaternion>
#include "openglfunctionsdebug.h"

//class QOpenGLTexture;
class ImageGrid;
class GridLayer;
class GridImage : public OpenGLFunctionsDebug {

public:
    GridImage();
    ~GridImage();
    QList< GridLayer *> m_gridLayers;

    QQuaternion q; // Position of image
    ImageGrid *m_imagegrid;
    GLuint m_squareVertexTextureCoordBuffer;
    GLuint *m_tilePositionBufferGrid;
    GLuint *m_tileTextureGrid;
    GLfloat **m_tiles;
    GLfloat *m_textureCoords;

//    QList<QOpenGLTexture *> m_tileTextureGridQt;

    bool m_initialized;
};


#endif // GRIDIMAGE_H
