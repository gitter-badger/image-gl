#ifndef GRIDLAYER_H
#define GRIDLAYER_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QPolygonF>
#include <QOpenGLFunctions>

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

#endif // GRIDLAYER_H
