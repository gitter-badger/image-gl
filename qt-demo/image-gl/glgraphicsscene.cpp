#include "glgraphicsscene.h"
#include <QGL>
#include <GL/glu.h>
#include <QTimer>

GLGraphicsScene::GLGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_backgroundColor = QColor(Qt::red);
}

GLGraphicsScene::~GLGraphicsScene()
{

}

void GLGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (painter->paintEngine()->type()
            != QPaintEngine::OpenGL) {
        qWarning("OpenGLScene: drawBackground needs a "
                 "QGLWidget to be set as viewport on the "
                 "graphics view");
        return;
    }

    glClearColor(m_backgroundColor.redF(),
                 m_backgroundColor.greenF(),
                 m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    if (!m_model)
//        return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
//    gluPerspective(70, width() / height(), 0.01, 1000);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    QPointF m_rotation;

    glTranslatef(0, 0, -1.0);
//    glRotatef(m_rotation.x, 1, 0, 0);
//    glRotatef(m_rotation.y, 0, 1, 0);
//    glRotatef(m_rotation.z, 0, 0, 1);

//    glEnable(GL_MULTISAMPLE);
//    m_model->render(m_wireframeEnabled, m_normalsEnabled);
//    glDisable(GL_MULTISAMPLE);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    QTimer::singleShot(20, this, SLOT(update()));
}
