#include "glgraphicsview.h"
#include <QResizeEvent>

GLGraphicsView::GLGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{

}

GLGraphicsView::~GLGraphicsView()
{

}

void GLGraphicsView::resizeEvent(QResizeEvent *event){
    if(scene()){
        scene()->setSceneRect(
                    QRect(QPoint(0,0),
                          event->size()));
    }
    QGraphicsView::resizeEvent(event);
}
