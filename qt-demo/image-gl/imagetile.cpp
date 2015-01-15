#include "imagetile.h"

ImageTile::ImageTile(QObject *parent) : QObject(parent)
{

}

ImageTile::~ImageTile()
{

}

QImage ImageTile::image()
{
    return m_image;
}

