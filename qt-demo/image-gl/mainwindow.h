#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include "gridwindow.h"

namespace Ui {
class MainWindow;
}

class BtDevice;
class ImageGrid;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static GridWindow *m_gridWindow ;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initBluetooth();

    bool loadImage(const QString &file);
    bool writeImage(QImage &img, QPoint pos, int row, int column, int dim);

    bool loadMultiple(const QString &path);

    int dimension();
    bool writeJSON(QJsonDocument &doc, QString path);

    QString format();

    void loadSettings();
    void saveSettings();


    void scanBluetooth();
private slots:
    void on_pushButtonSplit_clicked();
    void on_pushButtonLoadImage_clicked();
    void on_pushButtonDisplay_clicked();
    void on_pushButtonSelectImage_clicked();
    void on_pushButton_clicked();
    void on_pushButtonDemo2AP_clicked();
    void on_pushButtonDemo2LAT_clicked();
    void on_pushButtonDemo2Run_clicked();
    void on_pushButtonOsteotomy_clicked();
    void on_pushButtonSearch_clicked();

    void on_pushButtonLoad_clicked();

private:
    void updateLog(QStringList log);
    void updateErrors(QStringList errors);

    Ui::MainWindow *ui;

    /////////// Demo 1

    QString m_file;
    ImageGrid *m_grid;
    ImageGrid *m_grid2;

    /////////// Demo 2

    QString m_fileAP;
    QString m_fileLAT;

    ImageGrid *m_gridAP;
    ImageGrid *m_gridLAT;

    //////////// Demo 3: Multiple
    QString m_multiplePath;
};

#endif // MAINWINDOW_H
