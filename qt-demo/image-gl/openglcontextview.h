#ifndef OPENGLCONTEXTVIEW_H
#define OPENGLCONTEXTVIEW_H

#include <QObject>
#include <QWindow>

#include "openglfunctionsdebug.h"

class QOpenGLContext;
class QOpenGLPaintDevice;
class QPainter;
class QEvent;
class QExposeEvent;

class OpenGLContextView : public QWindow, protected OpenGLFunctionsDebug
{
public:
    explicit OpenGLContextView(QWindow *parent = 0);
    ~OpenGLContextView();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

    void setContext( QOpenGLContext *context );

protected:
    virtual bool event(QEvent *event) ;
    void exposeEvent(QExposeEvent *event) ;

private:
    bool m_update_pending;
    bool m_animating;
    bool m_initialized;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif // OPENGLCONTEXTVIEW_H
