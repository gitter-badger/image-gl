#include "gridlayer.h"
#include "gridimage.h"
//#include "gridwindow.h"

#include <QPolygonF>
#include <QMatrix4x4>

#include "graphics_util.h"

GridLayer::GridLayer( GridImage * g )
    :m_stencilVertices( NULL )
    ,m_gridImage( g )

{
}

GridLayer::~GridLayer()
{
    if( m_stencilVertices )
    {
        free( m_stencilVertices );
    }
}

void GridLayer::setPolygon( QPolygonF &poly ){

    m_stencilPolygon = poly;

    if( m_stencilVertices ){
        free( m_stencilVertices );
    }

    ///// Set up stencil vertices from layer polygon
    m_stencilVertices = ( GLfloat * )malloc( sizeof ( GLfloat ) * m_stencilPolygon.count() * 2 );
    int stencilVerticesCount = 0;
    foreach( QPointF point, m_stencilPolygon){
        QPointF pt = sm2gl( point, m_gridImage->m_imagegrid );
        m_stencilVertices[ stencilVerticesCount++ ] = ( float )pt.x();
        m_stencilVertices[ stencilVerticesCount++ ] = ( float )pt.y();
    }
}

QPolygonF GridLayer::stencilPolygon()
{
    return m_stencilPolygon;
}

void GridLayer::setTransformationMatrix(QMatrix4x4 &m)
{
    m_transformationMatrix = m;
}

QMatrix4x4 GridLayer::transformationMatrix()
{
    return m_transformationMatrix;
}

GLfloat *GridLayer::stencilVertices()
{
    return m_stencilVertices;
}

GridImage *GridLayer::gridImage()
{
    return m_gridImage;
}
