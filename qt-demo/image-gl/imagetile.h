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

    void setImage(QImage image);
    void unload();
signals:

public slots:
private:
    QImage m_image;
};

#endif // IMAGETILE_H
