#include "gridimage.h"
#include "imagegrid.h"
#include "imagetile.h"

#include "gridwindow.h"

//#include <QOpenGLTexture>

GridImage::GridImage(QObject *parent):
    QObject(parent)
    ,m_initialized(false)
    ,m_tilePositionBufferGrid( NULL )
    ,m_tileTextureGrid( NULL )
{

}

GridImage::~GridImage()
{
    /// Using QOpenGLTexture
    foreach(QOpenGLTexture *tex, m_tileTextureGridQt){
        delete tex;
    }

    /// Delete layers
//    foreach( GridLayer *layer, m_gridLayers ){
//        delete layer;
//    }

//    if(m_initialized){
//        qint64 count = m_imagegrid->rows() * m_imagegrid->cols();
//        glDeleteBuffers( count, m_tilePositionBufferGrid );
//        glDeleteBuffers( count, m_tileTextureGrid );
//        glDeleteBuffers( count, m_tileTextureGrid2 );
//        glDeleteBuffers( count, m_tileTextureGrid3 );
//        glDeleteBuffers( 1, &m_squareVertexTextureCoordBuffer );

//        free ( m_tileTextureGrid );
//        free ( m_tileTextureGrid2 );
//        free ( m_tileTextureGrid3 );
//        free ( m_tilePositionBufferGrid );


//    }
}

void GridImage::handleLoadedGridTexture(int index, int row, int col ){

    ImageTile *tile = m_imagegrid->tile( row, col );

    _handleLoadedTexture( tile->image(), m_tileTextureGrid [ GridWindow::tileIndex( row, col, m_imagegrid->cols() ) ], m_imagegrid->dimension() );

    /// Using QOpenGLTexture
//    QOpenGLTexture *tex = new QOpenGLTexture( tile->image().convertToFormat( QImage::Format_RGBA8888 ).mirrored( false, true ) );
//    tex->setMinificationFilter( QOpenGLTexture::Nearest );
//    tex->setMagnificationFilter( QOpenGLTexture::Nearest );
//    tex->setWrapMode( QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge );
//    tex->setWrapMode( QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge );
//    grid->m_tileTextureGridQt.insert( _tileIndex( row, col, imageGrid->cols() ), tex );

    Q_ASSERT( m_imagegrid->dimension() > 0 &&
              tile->image().width() == tile->image().height() &&
              tile->image().width() == m_imagegrid->dimension() );
}

void GridImage::_handleLoadedTexture( QImage image, GLuint texture, float dimension ){

    QImage img = image.convertToFormat( QImage::Format_RGBA8888 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, dimension, dimension, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.mirrored( false, true ).bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture( GL_TEXTURE_2D, 0 );
}
