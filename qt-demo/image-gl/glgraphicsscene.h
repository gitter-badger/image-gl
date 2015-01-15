#ifndef GLGRAPHICSSCENE_H
#define GLGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

class GLGraphicsScene : public QGraphicsScene, protected QOpenGLFunctions
{
public:
    GLGraphicsScene();
    ~GLGraphicsScene();

    virtual void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // GLGRAPHICSSCENE_H
