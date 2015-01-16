#include "mainwindow.h"
#include <QApplication>
#include "trianglewindow.h"
#include "gridwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("image-gl");
    QCoreApplication::setOrganizationName("klathzazt");
    QCoreApplication::setOrganizationDomain("klathzazt.com");
    QCoreApplication::setApplicationVersion(0);

    MainWindow w;
    w.show();

    return app.exec();
}
