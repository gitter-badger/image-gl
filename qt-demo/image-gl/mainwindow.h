#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool loadImage(const QString &file);
    bool writeImage(QImage &img, QPoint pos, int row, int column);

    int dimension();
    bool writeJSON(QJsonDocument &doc);

    QString format();

private slots:
    void on_pushButtonSplit_clicked();
    void on_pushButtonLoadImage_clicked();
    void on_pushButtonDisplay_clicked();

private:
    Ui::MainWindow *ui;
    QImage m_image;
    QString m_file;
};

#endif // MAINWINDOW_H
