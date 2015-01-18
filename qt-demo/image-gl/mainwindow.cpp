#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glgraphicsview.h"
#include "glgraphicsscene.h"
#include "ngraphicsscene.h"
#include "imagegrid.h"
#include "trianglewindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QJsonDocument>
#include <QGL>
#include <QSettings>

GridWindow *MainWindow::m_gridWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_grid(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();
}

void MainWindow::loadSettings(){
    QSettings s;
    QString selected = s.value("image/selected",QString()).toString();

    if(QFile::exists(selected)){
        ui->labelSelectedImage->setText(selected);
        m_file = selected;
    }
}

void MainWindow::saveSettings(){
    QSettings s;
    s.setValue("image/selected",m_file);
}

MainWindow::~MainWindow()
{
    delete m_gridWindow;
    delete ui;
}

bool MainWindow::loadImage(const QString &file){
    int dim = dimension();
    QString fmt = format();

    if(m_grid != NULL){
        delete m_grid;
    }
    m_grid = new ImageGrid(file, fmt, dim, this);
    if(m_grid->loadImage( dim )){
        updateLog(m_grid->logs());
    }else{
        updateLog(m_grid->errors());
    }
    bool ret = true;
    return ret;
}

int MainWindow::dimension(){
    return ui->comboBoxDimension->currentText().toInt();
}

void MainWindow::on_pushButtonSplit_clicked()
{
    if(!m_grid){
        QMessageBox::critical( this, QString( "Split error" ), QString( "Please load a file first" ));
        return;
    }

    bool ok =  m_grid->initTiles(dimension());

    if(!ok){
        QMessageBox::critical( this, QString( "Split error" ), QString( "Unable to init tiles" ));
        updateErrors(m_grid->errors());
    }else{
        ok = m_grid->writeTiles();
        if(!ok){
            QMessageBox::critical( this, QString( "Split error" ), QString( "Unable to write tiles" ));
            updateLog(m_grid->errors());
        }else{
            updateLog(m_grid->logs());
        }
    }
}

QString MainWindow::format()
{
    return ui->comboBoxFormat->currentText();
}

void MainWindow::updateLog(QStringList logs)
{
    QString text;

    foreach(QString log, logs){
        text.append(QString("%1\n").arg(log));
    }

    ui->textEditLog->setTextColor(QColor(Qt::black));
    ui->textEditLog->setText(text);
}

void MainWindow::updateErrors(QStringList errors)
{
    QString text;

    foreach(QString error, errors){
        text.append(QString("%1\n").arg(error));
    }

    ui->textEditLog->setTextColor(QColor(Qt::red));
    ui->textEditLog->setText(text);
}

void MainWindow::on_pushButtonSelectImage_clicked()
{
    QString file = QFileDialog::getOpenFileName( this, QString( "Select input file" ) );

    if(QFile::exists(file)){
        m_file = file;
        ui->labelSelectedImage->setText(m_file);
        saveSettings();
    }
}


void MainWindow::on_pushButtonLoadImage_clicked()
{
    if( QFile::exists( m_file ) ){
        ui->labelLoadedImage->setText( m_file );
        if(!loadImage( m_file )){
            updateErrors(m_grid->errors());
        }else{
            updateLog(m_grid->logs());
        }
    }
}

void MainWindow::on_pushButtonDisplay_clicked()
{
    if(m_gridWindow){
        delete m_gridWindow;
    }

    if(!m_grid){
        return;
    }

    m_gridWindow = new GridWindow();

    bool ok;
    ok = connect(m_grid, SIGNAL(tileImageLoaded(int,int)), m_gridWindow, SLOT( handleLoadedGridTexture(int,int) ));Q_ASSERT(ok);

    m_gridWindow->setGrid(m_grid);

    QSurfaceFormat format;
    format.setSamples(16);
//    format.setRenderableType(QSurfaceFormat::OpenGLES);

    m_gridWindow->setFormat(format);

    m_gridWindow->resize(1280, 768);
    m_gridWindow->setAnimating(true);
    m_gridWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow *wnd = new TriangleWindow();
    wnd->resize(640, 480);
    wnd->show();
    wnd->setAnimating(true);
}
