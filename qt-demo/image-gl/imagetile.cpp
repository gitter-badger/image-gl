#include "imagetile.h"

ImageTile::ImageTile(QObject *parent) : QObject(parent)
{

}

ImageTile::~ImageTile()
{
    m_image = QImage();
}

QImage ImageTile::image()
{
    return m_image;
}

void ImageTile::setImage(QImage image)
{
    m_image = image.copy();
}

