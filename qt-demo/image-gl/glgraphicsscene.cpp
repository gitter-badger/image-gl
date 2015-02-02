#include "glgraphicsscene.h"
#include <QPainter>
#include <QCoreApplication>
#include <QOpenGLContext>

GLGraphicsScene::GLGraphicsScene()
{

}

GLGraphicsScene::~GLGraphicsScene()
{

}

void GLGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    m_device->setSize(sceneRect().size().toSize());

    QPainter mPainter(m_device);
    render(&mPainter);
}

void GLGraphicsScene::initialize()
{

}

void GLGraphicsScene::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool GLGraphicsScene::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QGraphicsScene::event(event);
    }
}

void GLGraphicsScene::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

//    if (isExposed())
        renderNow();
}

void GLGraphicsScene::renderNow()
{
//    if (!isExposed())
//        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
//        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

//    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

//    render();

//    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void GLGraphicsScene::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}
