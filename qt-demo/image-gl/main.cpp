#include "mainwindow.h"
#include <QApplication>
#include "trianglewindow.h"

#include "imagegrid.h"
#include "gridwindow.h"




void guiApplicationTest(){
    ImageGrid *m_grid;
    QSurfaceFormat format;
    format.setSamples( 16 );
    format.setStencilBufferSize( 1 );
    QString m_file;

#ifdef Q_OS_ANDROID
    m_file = QString("/sdcard/DCIM/a.jpg");
#else
    m_file = QString("/Users/Jon/Desktop/5k_image.jpg");
#endif

    if(!QFile::exists(m_file)){
        return ;
    }

    m_grid = new ImageGrid( m_file, QString(), 1024, NULL );
    m_grid->loadImage();

    GridWindow *gridWindow1 = new GridWindow();

#ifdef Q_OS_ANDROID
    format.setRenderableType(QSurfaceFormat::OpenGLES);
#endif

    gridWindow1->setFormat( format );
    gridWindow1->addImage( m_grid );

    gridWindow1->resize( 1920, 1024 );
    gridWindow1->setAnimating( true );
    gridWindow1->showFullScreen();
    gridWindow1->fitToView();
}

int main(int argc, char *argv[])
{
//    QGuiApplication app(argc, argv);
//    app.setApplicationName("image-gl");
//    app.setOrganizationName("klathzazt");
//    app.setOrganizationDomain("klathzazt.com");
//    app.setApplicationVersion(0);
//    guiApplicationTest();

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("image-gl");
    QCoreApplication::setOrganizationName("klathzazt");
    QCoreApplication::setOrganizationDomain("klathzazt.com");
    QCoreApplication::setApplicationVersion(0);
    MainWindow w;
    w.show();

    return app.exec();
}
