#ifndef GLGRAPHICSVIEW_H
#define GLGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class QResizeEvent;

class GLGraphicsView : public QGraphicsView
{
public:
    GLGraphicsView(QWidget *parent = NULL);
    ~GLGraphicsView();

    virtual void paintEvent(QPaintEvent *event);
    void render(QPainter *painter, const QRectF &target = QRectF(), const QRect &source = QRect(),
                    Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);
    qreal m_zoom;
    qreal m_rotate;

    void updateZoomAndRotate(QSize size);

protected:
    void wheelEvent(QWheelEvent *wheel);
    void resizeEvent(QResizeEvent *event);

private:
    void scrollWithPixels(QPoint numPixels, Qt::KeyboardModifiers modifiers);
    void scrollWithDegrees(QPoint numSteps, Qt::KeyboardModifiers modifiers);
};

#endif // GLGRAPHICSVIEW_H
