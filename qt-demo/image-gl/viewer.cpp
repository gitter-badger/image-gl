#include "viewer.h"
#include <QDir>
#include <QFileInfoList>
#include <QImageReader>
#include <QWheelEvent>

#include <QSurfaceFormat>

#include "ui_viewer.h"
#include "gridimage.h"
#include "gridwindow.h"
#include "imagegrid.h"

class ViewerPrivate {
public:
    ViewerPrivate( Viewer *viewer );
    ~ViewerPrivate();

    QStringList nameFilters();
    QString error();
    void setNameFilters( QStringList list );
    void setError( const QString &error );

    void resetImageGridList();
    bool addImageGrid( ImageGrid * imageGrid );

    Ui::Viewer *m_ui;

    bool setCurrentImage();

    void nextImage();
    void prevImage();
private:
    QStringList m_nameFilters;
    QString m_error;
    Viewer *m_Owner;
    QList<ImageGrid *> m_imageGridList;

    ImageGrid *m_currentImage;
    QWidget *m_currentWidget;
    GridWindow *m_currentGridWindow;

};

Viewer::Viewer(QWidget *parent) : QWidget(parent),
    d(new ViewerPrivate( this ))
{

}

Viewer::~Viewer()
{
    delete d;
}

/**
 * @brief Viewer::setDirectory: Read entry list of matching file types
 * @param path
 * @return
 */
bool Viewer::setDirectory(const QString &path)
{
    bool result = false;

    QDir dir( path );
    if( !dir.exists() ){
        QString message = QString( "Directory doesn't exist" );
        d->setError( message );
        emit errorMessage( message );
        return false;
    }

    QFileInfoList list = dir.entryInfoList( d->nameFilters() );
    if( list.length() == 0 ){
        QString message = QString("Directory is empty");
        d->setError( message );
        emit errorMessage( message );
        return false;
    }

    d->resetImageGridList();

    foreach( QFileInfo info, list ){
        QString path = info.absoluteFilePath();
        ImageGrid *imageGrid = new ImageGrid( path, QString(), 1024, this );
        QString message;
        if( d->addImageGrid( imageGrid ) ){
            message = QString( "Image loaded: %1" ).arg( path );
            emit logMessage( message );
        }else{
            message = QString( "Image could not be loaded: %1" ).arg( path );
            emit errorMessage( message );
        }
    }

    d->setCurrentImage();

    return result;
}

QString Viewer::error() const
{
    return d->error();
}

void Viewer::wheelEvent( QWheelEvent *event )
{
    if(event->delta() < 0){
         prevImage();
    }else{
         nextImage();
    }
}

ViewerPrivate::ViewerPrivate(Viewer *viewer ):
    m_Owner( viewer )
    ,m_currentImage( NULL )
    ,m_currentWidget( NULL )
    ,m_ui( new Ui::Viewer )
{
    m_ui->setupUi(m_Owner);
    m_nameFilters.clear();
    foreach( QByteArray format, QImageReader::supportedImageFormats() ){
        m_nameFilters << QString( "*.%1" ).arg( QString( format ) );
    }
}

ViewerPrivate::~ViewerPrivate()
{
    delete m_ui;
    resetImageGridList();
}

QStringList ViewerPrivate::nameFilters()
{
    return m_nameFilters;
}

QString ViewerPrivate::error()
{
    return m_error;
}

void ViewerPrivate::setNameFilters( QStringList list )
{
    m_nameFilters = list;
}

void ViewerPrivate::setError( const QString &error )
{
    m_error = error;
}

void ViewerPrivate::resetImageGridList()
{
    foreach( ImageGrid *grid, m_imageGridList ){
        delete grid;
    }
    m_imageGridList.clear();
}

bool ViewerPrivate::addImageGrid( ImageGrid *imageGrid )
{
    bool result = false;
    if( imageGrid ){
        m_imageGridList << imageGrid;
        result = true;
    }
    return result;
}


void Viewer::nextImage()
{
    d->nextImage();
}

void Viewer::prevImage()
{
    d->prevImage();
}

bool Viewer::event(QEvent *e)
{
    if(e->type() == QEvent::Wheel){
        qDebug() << __FUNCTION__ << "WHEEL!";
    }
    return QWidget::event(e);
}

void ViewerPrivate::nextImage()
{
    if(m_imageGridList.count() == 0)
        return;

    ImageGrid *g = m_imageGridList.front();
    m_imageGridList.pop_front();
    m_imageGridList.push_back(g);
    setCurrentImage();
}

void ViewerPrivate::prevImage()
{
    if(m_imageGridList.count() == 0)
        return;

    ImageGrid *g = m_imageGridList.back();
    m_imageGridList.pop_back();
    m_imageGridList.push_front(g);
    setCurrentImage();
}

bool ViewerPrivate::setCurrentImage(){

    QVBoxLayout *v = m_ui->verticalLayout;

    if( m_currentImage ){
        v->removeWidget( m_currentWidget );
        m_currentImage->unloadImage();

        m_currentGridWindow->hide();
//        delete m_currentGridWindow;
    }

    if(m_imageGridList.count() == 0){
        return false;
    }
    ImageGrid *grid = m_imageGridList.first();

    grid->loadImage();

    m_currentImage = grid;

    QSurfaceFormat format;
    format.setSamples( 16 );
    format.setStencilBufferSize( 1 );

    GridWindow *gridWindow1 = new GridWindow();

    gridWindow1->setFormat( format );
    gridWindow1->addImage( grid );

    gridWindow1->resize( 800, 800 );
    gridWindow1->setAnimating( true );
    gridWindow1->fitToView();

    m_currentWidget = QWidget::createWindowContainer( gridWindow1, m_Owner );
    m_currentGridWindow = gridWindow1;
    v->addWidget(m_currentWidget);

    m_Owner->connect( gridWindow1, SIGNAL(prevImage()), SLOT(prevImage()));
    m_Owner->connect( gridWindow1, SIGNAL(nextImage()), SLOT(nextImage()));
    return true;
}
