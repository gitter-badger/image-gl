#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include "gridwindow.h"

namespace Ui {
class MainWindow;
}

class ImageGrid;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static GridWindow *m_gridWindow ;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool loadImage(const QString &file);
    bool writeImage(QImage &img, QPoint pos, int row, int column, int dim);

    int dimension();
    bool writeJSON(QJsonDocument &doc, QString path);

    QString format();

    void loadSettings();
    void saveSettings();

private slots:
    void on_pushButtonSplit_clicked();
    void on_pushButtonLoadImage_clicked();
    void on_pushButtonDisplay_clicked();

    void on_pushButtonSelectImage_clicked();

private:
    void updateLog(QStringList log);
    void updateErrors(QStringList errors);

    Ui::MainWindow *ui;
    QImage m_image;
    QString m_file;
    ImageGrid *m_grid;
};

#endif // MAINWINDOW_H
