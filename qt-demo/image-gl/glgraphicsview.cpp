#include "glgraphicsview.h"
#include <QResizeEvent>
#include <QGLWidget>

GLGraphicsView::GLGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    setViewport(new QGLWidget(
                         QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(
                QGraphicsView::FullViewportUpdate);
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
