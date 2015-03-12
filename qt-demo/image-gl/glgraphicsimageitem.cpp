#include "glgraphicsimageitem.h"
#include <QPainter>
#include <QDebug>

GLGraphicsImageItem::GLGraphicsImageItem(QImage image, QGraphicsItem *parent)
    :m_Image(image),
    QGraphicsItem(parent)
{

}

GLGraphicsImageItem::~GLGraphicsImageItem()
{

}

QPainterPath GLGraphicsImageItem::shape() const
{
    QPainterPath path;
    if (!m_Image.isNull()) {
        path.addRect(m_Image.rect());
    }
    return path;
}

QRectF GLGraphicsImageItem::boundingRect() const
{
    QRectF rect;
    if (!m_Image.isNull()) {
        rect = m_Image.rect();
    }
    return rect;
}

void GLGraphicsImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0,0,m_Image);
    qDebug() << __FUNCTION__ << m_Image.rect();
}

void GLGraphicsImageItem::setImage(const QImage image)
{
    m_Image = QImage();
    m_Image = image;
}

QVariant GLGraphicsImageItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}

