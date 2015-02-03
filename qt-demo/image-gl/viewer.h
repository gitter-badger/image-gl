#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>

namespace Ui {
class Viewer;
}

class ViewerPrivate;
class Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

    bool setDirectory(const QString &path);

    QString error() const;

public slots:
    void nextImage();
    void prevImage();

signals:
    void logMessage(const QString &);
    void errorMessage(const QString &);

protected:
    virtual bool event(QEvent *);
    virtual void wheelEvent(QWheelEvent *event);

private:
    ViewerPrivate *d;
};


#endif // VIEWER_H
