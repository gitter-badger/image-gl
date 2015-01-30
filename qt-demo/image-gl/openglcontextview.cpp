#include "openglcontextview.h"
#include <QPainter>
#include <QApplication>
#include <QOpenGLPaintDevice>
#include <QPainter>

OpenGLContextView::OpenGLContextView(QWindow *parent)
    : QWindow(parent)
    , m_context(NULL)
    , m_update_pending(false)
    , m_animating(false)
    , m_initialized(false)
    , m_device(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLContextView::~OpenGLContextView()
{
}

void OpenGLContextView::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLContextView::initialize()
{
}

void OpenGLContextView::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_device->setSize(size());

    QPainter painter(m_device);
    render(&painter);
}

void OpenGLContextView::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool OpenGLContextView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        break;
    }
    return QWindow::event(event);
}

void OpenGLContextView::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLContextView::renderNow()
{
    if ( !isExposed() )
        return;

    if ( m_context ){

        if(!m_initialized){
            initializeOpenGLFunctions();
            initialize();
            m_initialized = true;
        }

        m_context->makeCurrent(this);
        m_context->setFormat(requestedFormat());

        render();

        m_context->swapBuffers(this);
    }

    if ( m_animating )
        renderLater();
}

void OpenGLContextView::setContext(QOpenGLContext *context)
{
    m_context = context;
}

void OpenGLContextView::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}
