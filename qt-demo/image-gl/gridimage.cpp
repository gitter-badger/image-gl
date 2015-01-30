#include "gridimage.h"
#include "imagegrid.h"
//#include <QOpenGLTexture>

GridImage::GridImage():
    m_initialized(false),
    m_tilePositionBufferGrid( NULL )
  ,m_tileTextureGrid( NULL )
{

}

GridImage::~GridImage()
{
    /// Using QOpenGLTexture
//    foreach(QOpenGLTexture *tex, m_tileTextureGridQt){
//        delete tex;
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

//        foreach( GridLayer *layer, m_gridLayers ){
//            delete layer;
//        }
//    }
}
