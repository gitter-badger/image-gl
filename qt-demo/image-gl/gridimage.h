#ifndef GRIDIMAGE_H
#define GRIDIMAGE_H

#include <QObject>
#include <QQuaternion>
#include "openglfunctionsdebug.h"

//class QOpenGLTexture;
class ImageGrid;
class GridLayer;
class GridImage : public QObject, protected OpenGLFunctionsDebug {
    Q_OBJECT
public:
    GridImage(QObject *parent = NULL);
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

public slots:
    void handleLoadedGridTexture(int index, int row, int col);

private:
    void _handleLoadedTexture(QImage image, GLuint texture, float dimension);
};


#endif // GRIDIMAGE_H
