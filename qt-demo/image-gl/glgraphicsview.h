#ifndef GLGRAPHICSVIEW_H
#define GLGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class QResizeEvent;

class GLGraphicsView : public QGraphicsView
{
public:
    GLGraphicsView(QWidget *parent);
    ~GLGraphicsView();
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // GLGRAPHICSVIEW_H
