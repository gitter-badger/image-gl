#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGyroscope>
#include <QAccelerometer>
#include <QNetworkAccessManager>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void timeout();

private slots:
    void gyroChanged();
    void accelChanged();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer timer;
    QGyroscope gyro;
    QAccelerometer accel;
    QNetworkAccessManager mgr;

    bool m_running;
    QDateTime start;
};

#endif // MAINWINDOW_H
