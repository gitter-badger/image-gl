#ifndef GLGRAPHICSSCENE_H
#define GLGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

#include "openglfunctionsdebug.h"

class GLGraphicsScene : public QGraphicsScene, protected OpenGLFunctionsDebug
{
public:
    GLGraphicsScene();
    ~GLGraphicsScene();

    virtual void drawBackground(QPainter *painter, const QRectF &rect);

//    virtual void render(QPainter *painter);
//    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) ;
//    void exposeEvent(QExposeEvent *event) ;

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;






};

#endif // GLGRAPHICSSCENE_H
