#ifndef GLGRAPHICSSCENE_H
#define GLGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

class QExposeEvent;
class ImageGrid;
class GLGraphicsScenePrivate;
class GLGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GLGraphicsScene();
    ~GLGraphicsScene();


    virtual void initialize();

    void setAnimating(bool animating);
    void setImageGrid(ImageGrid *grid);

public slots:
    void renderLater();
    void renderNow();

    void handleLoadedGridTexture(int index, int row, int col);

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void exposeEvent(QExposeEvent *event);
    virtual bool event(QEvent *event) ;
//    void exposeEvent(QExposeEvent *event) ;

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;

    bool m_initialized;

    GLGraphicsScenePrivate *d;
};

#endif // GLGRAPHICSSCENE_H
