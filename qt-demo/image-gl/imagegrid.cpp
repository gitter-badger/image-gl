#include "imagegrid.h"
#include "imagetile.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QVariantMap>
#include <QJsonDocument>
#include <QDebug>

ImageGrid::ImageGrid(const QString &source, const QString &format, const int tileDimension, QObject *parent)
    :QObject(parent),
      m_cols(0),
      m_rows(0),
      m_file(source),
      m_format(format),
      m_index(-1),
      m_dimension(tileDimension)
{
}

ImageGrid::~ImageGrid()
{
    if(m_gridTiles.length() > 0){
        foreach(ImageTile *tile, m_gridTiles){
            delete tile;
        }
    }
}

qint64 ImageGrid::rows() const
{
    return m_rows;
}

qint64 ImageGrid::cols() const
{
    return m_cols;
}

ImageTile *ImageGrid::tile(qint64 row, qint64 col){
    ImageTile *val = NULL;

    int index = ( cols() * row ) + col;

    if( index < m_gridTiles.length() ){
        val = m_gridTiles.at( index );
    }
    return val;
}

bool ImageGrid::initTiles(qint64 dim)
{
    bool ok = false;

    m_dimension = dim;

    QFileInfo fileinfo ( m_file );
    QString filePath = fileinfo.absoluteDir().absolutePath();

    /// Create tiles
    QSize origsize   = m_image.size();

    int origwidth  = origsize.width();
    int origheight = origsize.height();

    // Amount to stretch by
    int wstretch = dim - (origwidth % dim);
    int hstretch = dim - (origheight % dim);

    // Add Stretch dimensions: padding pixels
    int stretchwidth = origwidth + wstretch;
    int stretchheight = origheight + hstretch;

    int rows = stretchheight / dim;
    int cols = stretchwidth  / dim;

    m_stretchwidth = stretchwidth;
    m_stretchheight = stretchheight;

    m_rows = rows;
    m_cols = cols;

    // Create tiles tiles
    for(qint64 i = 0; i < rows; i++){
        for(qint64 j = 0; j < cols; j++){
            ImageTile *tile = new ImageTile(this);
            m_gridTiles.append(tile);
        }
    }
    ok = true;

    qDebug() << __FUNCTION__
             << ((m_stretchwidth - origwidth)* 1.0) / (dim * 1.0)
             << ((m_stretchheight - origheight)* 1.0) / (dim * 1.0);

    return ok;
}

bool ImageGrid::writeTiles(){
    bool ok = true;
    QImage input = m_image.scaled( m_stretchwidth, m_stretchheight, Qt::KeepAspectRatio, Qt::SmoothTransformation );

    QImage smallImage;

    qint64 dim = dimension();

    int small_dim = 128;

    switch( dim ){
    case 256:
    case 512:
        // don't make smaller copies;
        break;
    case 1024:
        smallImage = m_image.scaled( m_stretchwidth / 8, m_stretchheight / 8, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        break;
    case 2048:
        smallImage = m_image.scaled( m_stretchwidth / 16, m_stretchheight / 16, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        break;
    case 4096:
        smallImage = m_image.scaled( m_stretchwidth / 32, m_stretchheight / 32, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        break;
    case 8192:
        smallImage = m_image.scaled( m_stretchwidth / 64, m_stretchheight / 64, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        break;
    }

    QPoint offset = QPoint( 0,0 );

    // Make small image tiles small_dim x small_dim
    if(!smallImage.isNull()){
        for( int row = 0; row < rows(); row++ ){
            for( int col = 0; col < cols(); col++ ){
                offset = QPoint( dim * col, dim * row );
                QImage img = input.copy( offset.x(), offset.y(), dim, dim );
                QString message;

                ok = _writeImage( img, offset, row, col, small_dim );
                if(ok){
                    message = QString( "%1 %2 %3 %4" ).arg( offset.x() ).arg( offset.y() ).arg( dim ).arg( dim );
                    _log(message);
                }else{
                    message = QString( "Problem writing file" );
                    _error(message);
                }
            }
        }
    }

    // Full Size image
    for( int row = 0; row < rows(); row++ ){
        for( int col = 0; col < cols(); col++ ){
            offset = QPoint( dim * col, dim * row );
            QImage img = input.copy( offset.x(), offset.y(), dim, dim );

            QString message;
            ok = _writeImage( img, offset, row, col, m_dimension );

            if( ok ){
                message = QString( "%1 %2 %3 %4" ).arg( offset.x() ).arg( offset.y() ).arg( dim ).arg( dim );
                _log(message) ;
            }else{
                message = QString( "Problem writing file" );
                _error(message);
            }
        }
    }



    if(ok){
        QVariantMap map;

        map[ "stretchWidth"]  = m_stretchwidth;
        map[ "stretchHeight"] = m_stretchheight;
        map[ "width" ]        = m_image.width();
        map[ "height" ]       = m_image.height();
        map[ "rows" ]         = rows();
        map[ "cols" ]         = cols();
        map[ "dimension" ]    = dim;
        map[ "dimensionSmall" ] = small_dim;
        map[ "format" ]       = format();

        QJsonDocument jsondoc = QJsonDocument::fromVariant( map );

        ok = _writeJSON( jsondoc );
        if(!ok){
            QString message = QString( "Problem writing data file" ) ;
            _error ( message );
        }
    }

    return ok;
}

bool ImageGrid::loadTiles(){
    bool ok = true;
    QImage input = m_image.scaled( m_stretchwidth, m_stretchheight, Qt::KeepAspectRatio, Qt::SmoothTransformation );

    qint64 dim = dimension();

    QPoint offset = QPoint( 0,0 );

//     Full Size image
    for( int row = 0; row < rows(); row++ ){
        for( int col = 0; col < cols(); col++ ){
            offset = QPoint( dim * col, dim * row );
            QImage img = input.copy( offset.x(), offset.y(), dim, dim );

            QString message;
            //ok = _writeImage( img, offset, row, col, m_dimension );

            ImageTile *currentTile = tile( row, col );

            currentTile->setImage( img.copy() );

            emit tileImageLoaded(m_index, row, col );

            if( ok ){
                message = QString( "%1 %2 %3 %4" ).arg( offset.x() ).arg( offset.y() ).arg( dim ).arg( dim );
                _log(message) ;
            }else{
                message = QString( "Problem writing file" );
                _error(message);
            }
        }
    }

    return ok;
}

const QString ImageGrid::format() const
{
    return m_format;
}

const qint64 ImageGrid::dimension() const
{
    return m_dimension;
}

QStringList ImageGrid::errors() const
{
    return m_error;
}

QStringList ImageGrid::logs() const
{
    return m_log;
}

const bool ImageGrid::_writeJSON( const QJsonDocument &doc )
{
    bool ok = false;

    QString filename = QString( "%1%2tile_data.js" ).
            arg( filePath() ).
            arg( QDir::separator() );

    QFile wdevice( filename );
    if( !wdevice.open( QIODevice::WriteOnly ) ) {
        QString message = QString( "Could not open file for writing");
        _error(message);
        ok = false;
    }else{
        wdevice.write( "var tileImage = " );
        qint64 length = wdevice.write( doc.toJson() );
        wdevice.close();
        ok = length > 0;
    }
    return ok;
}

QString ImageGrid::filePath(){
    QFileInfo fileinfo( m_file );
    return fileinfo.absoluteDir().absolutePath();
}

void ImageGrid::setIndex(qint64 index)
{
    m_index = index;
    qDebug() << __FUNCTION__ << index;
}

bool ImageGrid::_writeImage(QImage &img, QPoint pos, int row, int column, int dim){

    Q_UNUSED(pos);

    bool ok = false;

    QString filename =
            QString( "%1%2tile_%3%4_%5.%6" ).
            arg( filePath() ).
            arg( QDir::separator() ).
            arg( row ).
            arg( column ).
            arg( dim ).
            arg( format() );

    QFile wdevice(filename);
    if ( !wdevice.open( QIODevice::WriteOnly ) ) {
        QString message = QString( "Could not write file %1" ).arg( filename );
        _error(message);
        ok = false;
    }else{
        QImageWriter writer( &wdevice, format().toLatin1() );

        writer.setCompression( 0 );
        writer.setQuality( 100 );
        ok = writer.write( img );
        if(!ok){
            QString message = QString( "Write failed on file %1" ).arg( filename );
            _error(message);
        }
        wdevice.close();
    }
    return ok;
}

bool ImageGrid::loadImage( qint64 dim ){

    QFile device( m_file );
    bool ret = false;
    if ( !device.open( QIODevice::ReadOnly ) ) {
        QString message = QString( "Could not load image selected." );
        _error ( message );
        ret = false;
    } else {
        QImageReader *reader = new QImageReader( &device );
        reader->setDecideFormatFromContent( true );
        if ( reader->canRead() ) {
            if ( !reader->read( &m_image ) ) {
                QString message =  QString("Could not load image selected: %1 ").arg(reader->errorString());
                _error ( message );
            }else{
                ret = true;
                QString message = QString("Image loaded successfully");
                _log( message );
            }
        }
        delete reader;
    }

    initTiles( dim );

    return ret;
}

void ImageGrid::_log(const QString &message)
{
    m_log << message;
}

void ImageGrid::_error(const QString &message)
{
    m_error << message;
}
