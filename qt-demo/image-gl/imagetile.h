#ifndef IMAGETILE_H
#define IMAGETILE_H

#include <QObject>
#include <QImage>
class ImageTile : public QObject
{
    Q_OBJECT
public:
    explicit ImageTile(QObject *parent = 0);
    ~ImageTile();

    QImage image();
    QImage mediumresImage();
    QImage lowresImage();

    void setImage(QImage image);
signals:

public slots:
private:
    QImage m_lowresImage;
    QImage m_mediumresImage;
    QImage m_image;
};

#endif // IMAGETILE_H
