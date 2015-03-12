#include "glgraphicsview.h"
#include "glgraphicsscene.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QGLWidget>

GLGraphicsView::GLGraphicsView(QWidget *parent):
    QGraphicsView(parent),
    m_zoom(100.0),
    m_rotate(0.0)
{
    setViewport(new QGLWidget(
                         QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(
                QGraphicsView::FullViewportUpdate);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

GLGraphicsView::~GLGraphicsView()
{

}

void GLGraphicsView::paintEvent(QPaintEvent *event)
{
    if(!this->scene()){
        QAbstractScrollArea::paintEvent(event);
        return;
    }

    QRegion exposed = event->region();
    QRectF exposedSceneRect = mapToScene(exposed.boundingRect()).boundingRect();

    // Paint scene and image using QGraphicsView
    QRectF viewportRect = QRectF(viewport()->rect());
    QPainter painter(viewport());

    if(GLGraphicsScene *s = dynamic_cast<GLGraphicsScene*>(scene())){
        QGLWidget *glwidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
        QPainter imagePainter(&glwidget);
        imagePainter.setRenderHint(QPainter::Antialiasing);
        s->render( &imagePainter );
//        s->setImage( image );
    }
    QGraphicsView::paintEvent(event);

//    render(&painter);
}

void GLGraphicsView::render(QPainter *painter, const QRectF &target, const QRect &source, Qt::AspectRatioMode aspectRatioMode)
{
    QGraphicsView::render( painter, target, source, aspectRatioMode );
}

void GLGraphicsView::resizeEvent(QResizeEvent *resizeEvent){
    QGraphicsView::resizeEvent(resizeEvent);
    if(scene()){
        scene()->setSceneRect(
                    QRect(QPoint(0,0),
                          resizeEvent->size()));
        QSize size = resizeEvent->size();
        updateZoomAndRotate(size);
        resizeEvent->accept();
    }
}

/**
 * @brief QsmGraphicsView::updateZoomAndRotate
 */
void GLGraphicsView::updateZoomAndRotate(QSize size)
{
    QGraphicsScene *s = scene();
    if( GLGraphicsScene *scene = dynamic_cast< GLGraphicsScene * >( s ) ){
        QImage image = scene->image();
        if( ! image.isNull() ){
            QSizeF vSize = size;
            if( size.isEmpty() ){
                vSize = viewport()->size();
            }
            // Don't do an update if the viewport isn't even visible
            if( vSize.width() == 0 || vSize.height() == 0 )
                return;
            QSizeF iSize = image.size();
            qreal factor = qMin< qreal >( vSize.height() / iSize.height(), vSize.width() / iSize.width() );

            qreal z = factor * (m_zoom) / 100;
            int rotate = m_rotate;

            QPoint cursor = viewport()->mapFromGlobal( QCursor::pos() );
            QPointF oldCursor, oldCenter;
            bool center = geometry().contains( cursor );
            if (center ) {
                oldCenter = mapToScene( viewport()->geometry().center() );
                oldCursor = mapToScene( cursor );
            }

            resetTransform();
            scale( z, z );
            QGraphicsView::rotate( rotate );
        }
    }
}

void GLGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    if(!numPixels.isNull()){
        scrollWithPixels(numPixels,event->modifiers());
    }else if(!numDegrees.isNull()){
        QPoint numSteps = numDegrees / 5;
        scrollWithDegrees(numSteps,event->modifiers());
    }
    updateZoomAndRotate(QSize());
    event->accept();
}

void GLGraphicsView::scrollWithPixels(QPoint numPixels, Qt::KeyboardModifiers modifiers) {

    bool scroll_to_zoom = true; // QsmSettingsManager::getUserSetting(QString("application/scroll_to_zoom"),false).toBool();

    if(modifiers & Qt::ControlModifier || scroll_to_zoom){
//        if(d->m_ImageSettings){
//            d->m_ImageSettings->setZoom( (imageWidget()->view()->zoom() + numpixels.y()));
//        }
        m_zoom += numPixels.y();
    }else{
//        if(numpixels.y() > 0){
//            next();
//        }else{
//            if(numpixels.y() < 0){
//                prev();
//            }
//        }
    }
}

void GLGraphicsView::scrollWithDegrees(QPoint numSteps, Qt::KeyboardModifiers modifiers){

    bool scroll_to_zoom = true; // QsmSettingsManager::getUserSetting(QString("application/scroll_to_zoom"),false).toBool();

    if(modifiers & Qt::ControlModifier || scroll_to_zoom){
//        if(d->m_ImageSettings){
//            d->m_ImageSettings->setZoom( (imageWidget()->view()->zoom() + numSteps.y()));
//        }
        m_zoom += numSteps.y();
    }else{
//        if(numSteps.y() > 0){
//            next();
//        }else{
//            if(numSteps.y() < 0){
//                prev();
//            }
//        }
    }
}

