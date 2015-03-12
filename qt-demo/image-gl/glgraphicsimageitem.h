#ifndef GLGRAPHICSIMAGEITEM_H
#define GLGRAPHICSIMAGEITEM_H

#include <QGraphicsItem>

class GLGraphicsImageItem : public QGraphicsItem
{
public:
    GLGraphicsImageItem(QImage image = QImage(), QGraphicsItem *parent = 0);
    ~GLGraphicsImageItem();

    enum { Type = UserType + 33211 };

    int type() const { return Type; }

    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget = 0);

    void setImage(const QImage image);

protected:
    QVariant itemChange (GraphicsItemChange change, const QVariant &value);

private:
    QImage m_Image;
};

#endif // GLGRAPHICSIMAGEITEM_H
