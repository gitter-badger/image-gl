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
    m_gridAP (NULL),
    m_gridLAT (NULL),
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

    QString selectedAP = s.value("image/selectedAP",QString()).toString();

    if(QFile::exists(selectedAP)){
        ui->labelAP->setText(QString("AP: %1").arg(selectedAP));
        m_fileAP = selectedAP;
    }

    QString selectedLAT = s.value("image/selectedLAT",QString()).toString();

    if(QFile::exists(selectedLAT)){
        ui->labelLAT->setText(QString("LAT: %1").arg(selectedLAT));
        m_fileLAT = selectedLAT;
    }
}

void MainWindow::saveSettings(){
    QSettings s;
    s.setValue("image/selected",m_file);
    s.setValue("image/selectedLAT",m_fileLAT);
    s.setValue("image/selectedAP",m_fileAP);
}

MainWindow::~MainWindow()
{
//    delete m_gridWindow;
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

void MainWindow::on_pushButton_clicked()
{
    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow *wnd = new TriangleWindow();
    wnd->resize(640, 480);
    wnd->show();
    wnd->setAnimating(true);
}

void MainWindow::on_pushButtonDemo2AP_clicked()
{
    QString file = QFileDialog::getOpenFileName( this, QString( "Select AP file" ) );

    if(QFile::exists(file)){
        m_fileAP = file;
        ui->labelAP->setText(QString("AP: %1").arg(m_fileAP));
        saveSettings();
    }
}

void MainWindow::on_pushButtonDemo2LAT_clicked()
{
    QString file = QFileDialog::getOpenFileName( this, QString( "Select LAT file" ) );

    if(QFile::exists(file)){
        m_fileLAT = file;
        ui->labelLAT->setText(QString("LAT: %1").arg(m_fileLAT));
        saveSettings();
    }
}

void MainWindow::on_pushButtonOsteotomy_clicked()
{
    if(m_gridWindow){
        delete m_gridWindow;
        m_gridWindow = NULL;
    }

    m_gridWindow = new GridWindow();

    QQuaternion q = QQuaternion(0,0,0,0);

    int dim = dimension();
    QString fmt = format();


    m_gridAP = new ImageGrid(m_fileLAT, fmt, dim, this);
    if(m_gridAP->loadImage( dim )){
        updateLog(m_gridAP->logs());
    }else{
        updateLog(m_gridAP->errors());
    }

    m_gridLAT = new ImageGrid(m_fileLAT, fmt, dim, this);
    if(m_gridLAT->loadImage( dim )){
        updateLog(m_gridLAT->logs());
    }else{
        updateLog(m_gridLAT->errors());
    }

    QVector3D translate0(0.0, -0.1, 0);
    QVector3D translate1(-0.1, 0.1, 0);

    qreal zrotation0 = 12.0f;
    qreal zrotation1 = -12.0f;

    m_gridWindow->addImage( m_gridAP );
    m_gridWindow->addImage( m_gridLAT );

    QSurfaceFormat format;
    format.setSamples(16);

    m_gridWindow->setFormat(format);

    m_gridWindow->resize(1280, 768);
    m_gridWindow->setAnimating(true);
    m_gridWindow->show();
}

void MainWindow::on_pushButtonDisplay_clicked()
{
    on_pushButtonLoadImage_clicked();

    if(!m_grid){
        return;
    }

    GridWindow *gridWindow = new GridWindow();
    gridWindow->addImage( m_grid );

    QSurfaceFormat format;
    format.setSamples(16);
    format.setStencilBufferSize(1);

    gridWindow->setFormat(format);

    gridWindow->resize(800, 800);
    gridWindow->setAnimating(true);
    gridWindow->show();

//    QDialog dlg;
//    dlg.setLayout(new QVBoxLayout());
//    QWidget *widget = QWidget::createWindowContainer(gridWindow, &dlg);
//    dlg.resize(800, 800);
//    dlg.layout()->addWidget(widget);
//    gridWindow->fitToView();
//    dlg.exec();
}

void MainWindow::on_pushButtonDemo2Run_clicked()
{
    if(m_gridWindow){
        delete m_gridWindow;
        m_gridWindow = NULL;
    }

    int dim = dimension();
    QString fmt = format();

    if(m_gridAP != NULL){
        delete m_gridAP;
    }
    if(m_gridLAT != NULL){
        delete m_gridLAT;
    }

    qDebug() << __FUNCTION__ << "AP: " << m_fileAP;
    qDebug() << __FUNCTION__ << "LAT:" << m_fileLAT;

    ////////// Load image grids

    m_gridAP = new ImageGrid(m_fileAP, fmt, dim, this);
    bool apOk = m_gridAP->loadImage( dim );
    if(apOk){
        updateLog(m_gridAP->logs());
    }else{
        updateLog(m_gridAP->errors());
    }

    m_gridLAT = new ImageGrid(m_fileLAT, fmt, dim, this);
    bool latOk = m_gridLAT->loadImage( dim );
    if(latOk){
        updateLog(m_gridLAT->logs());
    }else{
        updateLog(m_gridLAT->errors());
    }

    if(!latOk || !apOk){
        return;
    }

    QQuaternion q = QQuaternion(0.0, 0.0, 0.0, 0.0);
    QQuaternion q2 = QQuaternion( 90.0, 0.0, 1.0, 0.0);

    m_gridWindow = new GridWindow();

    m_gridWindow->addImage(m_gridAP,q);
    m_gridWindow->addImage(m_gridLAT,q2);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setStencilBufferSize(8);

//    format.setRenderableType(QSurfaceFormat::OpenGLES);

    m_gridWindow->setVFlip90(true);
    m_gridWindow->setFormat(format);

    m_gridWindow->resize(1280, 768);
    m_gridWindow->setAnimating(true);
    m_gridWindow->show();
}
