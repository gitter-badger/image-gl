#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QJsonDocument>
#include "glgraphicsview.h"
#include "glgraphicsscene.h"
#include <QGL>

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
    QFile device( file );
    bool ret = false;
    if ( !device.open( QIODevice::ReadOnly ) ) {
        QMessageBox::critical( this, QString( "Problem loading file: 1" ), QString( "Could not load image selected." ));
        ret = false;
    } else {
        QImageReader *reader = new QImageReader( &device );
        reader->setDecideFormatFromContent( true );
        if ( reader->canRead() ) {
            if ( !reader->read( &m_image ) ) {
                QMessageBox::critical( this, QString( "Problem loading file: 2" ), QString("Could not load image selected: %1 ").arg(reader->errorString()));
            }else{
                ret = true;
            }
        }
        delete reader;
    }
    return ret;
}

int MainWindow::dimension(){
    return ui->comboBoxDimension->currentText().toInt();
}

void MainWindow::on_pushButtonSplit_clicked()
{
    if(m_image.isNull()){
        QMessageBox::critical( this, QString( "Split error: 1" ), QString( "Image is NULL" ));
        return;
    }

    // width & height of tile
    int dim = dimension();

    /// Create tiles

    QSize origsize   = m_image.size();

    int origwidth  = origsize.width();
    int origheight = origsize.height();

    // Amount to stretch by
    int wstretch = dim - (origwidth % dim);
    int hstretch = dim - (origheight % dim);

    // Add Stretch dimensions
    int stretchwidth = origwidth + wstretch;
    int stretchheight = origheight + hstretch;

    int rows = stretchheight / dim;
    int cols = stretchwidth  / dim;

    QImage input = m_image.scaled( stretchwidth, stretchheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    QPoint offset = QPoint( 0,0 );

    bool ok = false;
    for( int row = 0; row < rows; row++ ){
        for( int col = 0; col < cols; col++ ){
            offset = QPoint( dim * col, dim * row );
            QImage img = input.copy( offset.x(), offset.y(), dim, dim );

            QString message;
            ok = writeImage( img, offset, row, col );
            if(ok){
                message = QString( "%1 %2 %3 %4" ).arg( offset.x() ).arg( offset.y() ).arg( dim ).arg( dim );
                ui->textEditLog->append( message );
            }else{
                message = QString( "Problem writing file" );
            }
            ui->textEditLog->append( message );
            statusBar()->showMessage( message );
        }
    }
    if(ok){
        QVariantMap map;

        map[ "stretchWidth"]  = stretchwidth;
        map[ "stretchHeight"] = stretchheight;
        map[ "width" ]        = origwidth;
        map[ "height" ]       = origheight;
        map[ "rows" ]         = rows;
        map[ "cols" ]         = cols;
        map[ "dimension" ]    = dim;
        map[ "format" ]       = format();

        QJsonDocument jsondoc = QJsonDocument::fromVariant( map );
        ok = writeJSON( jsondoc );
        if(!ok){
            QMessageBox::critical( this, QString( "Problem writing data file" ), QString( "Problem writing data file" ) );
        }
    }
}

bool MainWindow::writeJSON( QJsonDocument &doc ){
    bool ok = false;
    QFileInfo fileinfo (m_file );
    QString filename = QString( "%1%2tile_data.js" ).
            arg( fileinfo.absoluteDir().absolutePath() ).
            arg( QDir::separator() );

    QFile wdevice( filename );
    if( !wdevice.open( QIODevice::WriteOnly ) ) {
        QMessageBox::critical( this, QString( "Problem writing tile data file" ), QString( "Could not open file for writing") );
        ok = false;
    }else{
        wdevice.write( "var tileImage = " );
        qint64 length = wdevice.write( doc.toJson() );
        wdevice.close();
        ok = length > 0;
    }
    return ok;
}

QString MainWindow::format()
{
    return ui->comboBoxFormat->currentText();
}

bool MainWindow::writeImage(QImage &img, QPoint pos, int row, int column){

    Q_UNUSED(pos);

    bool ok = false;

    int dim = dimension();

    QFileInfo fileinfo( m_file );
    QString filename =
            QString( "%1%2tile_%3%4_%5.%6" ).
            arg( fileinfo.absoluteDir().absolutePath() ).
            arg( QDir::separator() ).
            arg( row ).
            arg( column ).
            arg( dim ).
            arg( format() );


    QFile wdevice(filename);
    if ( !wdevice.open( QIODevice::WriteOnly ) ) {
        QMessageBox::critical( this, QString( "Problem writing file: 1" ), QString( "Could not write file %1" ).arg( filename ) );
        ok = false;
    }else{
        QImageWriter writer( &wdevice, format().toLatin1() );

        writer.setCompression( 0 );
        writer.setQuality( 100 );
        ok = writer.write( img );
        if(!ok){
            QMessageBox::critical( this, QString( "Problem writing file: 2" ), QString( "Could not write file %1" ).arg( filename ) );
        }
        wdevice.close();
    }
    return ok;
}

void MainWindow::on_pushButtonLoadImage_clicked()
{
    m_file = QFileDialog::getOpenFileName( this, QString( "Select file to split" ) );

    if( QFile::exists( m_file ) ){
        ui->labelImageLoaded->setText( QString("File Loaded: %1" ).arg( m_file ));
        loadImage( m_file );
    }
}

void MainWindow::on_pushButtonDisplay_clicked()
{
    QDialog dlg;
    QVBoxLayout layout;
    dlg.setLayout(&layout);
    GLGraphicsView glview(&dlg);
    GLGraphicsScene glscene(this);

    glview.setViewport(new QGLWidget(
        QGLFormat(QGL::SampleBuffers)));
    glview.setViewportUpdateMode(
        QGraphicsView::FullViewportUpdate);
    glview.setScene(&glscene);
    glview.show();

    layout.addWidget(&glview);

    QSize size(500,500);
    dlg.resize(size);
    dlg.exec();
}
