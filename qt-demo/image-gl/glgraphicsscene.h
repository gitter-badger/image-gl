#ifndef GLGRAPHICSSCENE_H
#define GLGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QColor>

class GLGraphicsScene : public QGraphicsScene
{
public:
    GLGraphicsScene(QObject *parent = NULL);
    ~GLGraphicsScene();

    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QColor m_backgroundColor;
};

#endif // GLGRAPHICSSCENE_H
