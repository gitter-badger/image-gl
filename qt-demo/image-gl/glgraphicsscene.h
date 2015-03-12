#ifndef GLGRAPHICSSCENE_H
#define GLGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
//#include <QOpenGLFunctions>
//#include <QOpenGLPaintDevice>

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

    void render( QPainter *painter, const QRectF &target = QRectF(), const QRectF &source = QRectF(), Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio );
    void setImageGrid(ImageGrid *grid);

    QImage image();
    void setImage(QImage image);

public slots:

    void handleLoadedGridTexture(int index, int row, int col);

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual bool event(QEvent *event) ;

private:
    bool m_update_pending;
    bool m_animating;
    bool m_initialized;

    GLGraphicsScenePrivate *d;
};

#endif // GLGRAPHICSSCENE_H
