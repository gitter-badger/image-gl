#include "imagetile.h"

ImageTile::ImageTile(QObject *parent) : QObject(parent)
{

}

ImageTile::~ImageTile()
{
    m_mediumresImage = QImage();
    m_lowresImage = QImage();
    m_image = QImage();
}

QImage ImageTile::image()
{
    return m_image;
}

QImage ImageTile::mediumresImage(){
    return m_mediumresImage;
}

QImage ImageTile::lowresImage(){
    return m_lowresImage;
}



void ImageTile::setImage(QImage image)
{
    m_image = image.copy();
    m_mediumresImage = image.scaled(m_image.size() * (1.0 / 4.0));
    m_lowresImage = image.scaled(m_image.size() * (1.0 / 8.0));
}

