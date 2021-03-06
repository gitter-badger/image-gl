#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QExposeEvent>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include "openglfunctionsdebug.h"

class OpenGLWindow : public QWindow, protected OpenGLFunctionsDebug
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    virtual bool event(QEvent *event) ;
    void exposeEvent(QExposeEvent *event) ;

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};



#endif // OPENGLWINDOW_H
