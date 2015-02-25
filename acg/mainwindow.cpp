#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDateTime>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_running(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool ok = connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));Q_ASSERT(ok);
    if(!ok){
        exit(0);
    }
    timer.setInterval( 20 );

    ok = connect(&gyro,  SIGNAL(readingChanged()), SLOT(gyroChanged()));  Q_ASSERT(ok);
    ok = connect(&accel, SIGNAL(readingChanged()), SLOT(accelChanged())); Q_ASSERT(ok);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout(){

    qreal msecs = start.msecsTo(QDateTime::currentDateTime());

    QAccelerometerReading *acr = accel.reading();
    QGyroscopeReading     *gyr = gyro.reading();

    QString accelstring;
    if(acr){
        accelstring = QString("%1,%2,%3")
                .arg(acr->x())
                .arg(acr->y())
                .arg(acr->z());
    }

    QString gyrostring;
    if(gyr){
        gyrostring = QString("%1,%2,%3")
                .arg(gyr->x())
                .arg(gyr->y())
                .arg(gyr->z());
    }


    QString text = QString("%1,%2,%3").arg(msecs).arg(accelstring).arg(gyrostring);

    qDebug() << text;
    ui->plainTextEdit->appendPlainText(text);

    QString url = QString("http://10.0.10.46:8080/%1").arg(text);
    QNetworkRequest *req = new QNetworkRequest(QUrl(url));

    mgr.get(*req);
}

void MainWindow::gyroChanged()
{

}

void MainWindow::accelChanged()
{

}

void MainWindow::on_pushButton_clicked()
{
    if(!m_running){
        start = QDateTime::currentDateTime();
        gyro.start();
        accel.start();
        timer.start();
        ui->pushButton->setText("Stop");
        m_running = true;
    }else{
        timer.stop();
        gyro.stop();
        accel.stop();
        ui->pushButton->setText("Start");
        m_running = false;
    }
}
