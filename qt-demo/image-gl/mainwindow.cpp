#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadImage(const QString &file){
    QFile device(file);
    bool ret = false;
    if (!device.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, QString("Problem loading file: 1"), QString("Could not load image selected."));
        ret = false;
    } else {
        QImageReader *reader = new QImageReader(&device);
         reader->setDecideFormatFromContent(true);
        if (reader->canRead()) {
            if (!reader->read(&m_image)) {
                QMessageBox::critical(this, QString("Problem loading file: 2"), QString("Could not load image selected."));
            }else{
                ret = true;
            }
        }
        delete reader;
    }
    return ret;
}

void MainWindow::on_pushButtonSplit_clicked()
{
    if(m_image.isNull()){
        QMessageBox::critical(this, QString("Split error: 1"), QString("Image is NULL"));
        return;
    }

    int dimension = ui->comboBoxDimension->currentText().toInt();


    // Create tiles
    QSize size   = m_image.size();
    int   width  = size.width();
    int   height = size.height();

    int wstretch = width % dimension;
    int hstretch = height % dimension;

    width += wstretch;
    height += hstretch;

    int cols = width / dimension;
    int rows = height / dimension;

    QImage input = m_image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPoint offset = QPoint(0,0);

    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            offset = QPoint(dimension * col, dimension * row);
            QImage img = input.copy(offset.x(), offset.y(), dimension, dimension);

            QString message;
            bool ok = writeImage(img, offset, row, col);
            if(ok){
                message = QString("%1 %2 %3 %4").arg(offset.x()).arg(offset.y()).arg(dimension).arg(dimension);
                ui->textEditLog->append(message);
            }else{
                message = QString("Problem writing file");
            }
            ui->textEditLog->append(message);
            statusBar()->showMessage(message);
        }
    }
}

bool MainWindow::writeImage(QImage &img, QPoint pos, int row, int column){
    bool ok = false;
    QString format = QString("jpg");

    QFileInfo fileinfo(m_file);
    QString filename =
            QString("%1%2%3%4.%5").
            arg(fileinfo.absoluteDir().absolutePath()).
            arg(QDir::separator()).
            arg(row).
            arg(column).
            arg(format);


    QFile wdevice(filename);
    if (!wdevice.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, QString("Problem writing file: 1"), QString("Could not write file %1").arg(filename));
        ok = false;
    }else{
        QImageWriter writer(&wdevice, format.toLatin1());

        writer.setCompression(0);
        writer.setQuality(100);
        ok = writer.write(img);
        if(!ok){
            QMessageBox::critical(this, QString("Problem writing file: 2"), QString("Could not write file %1").arg(filename));
        }
    }
    return ok;
}

void MainWindow::on_pushButtonLoadImage_clicked()
{
    m_file = QFileDialog::getOpenFileName(this, QString("Select file to split"));

    if(QFile::exists(m_file)){
        ui->labelImageLoaded->setText(QString("File Loaded: %1").arg(m_file));
        loadImage(m_file);
    }
}
