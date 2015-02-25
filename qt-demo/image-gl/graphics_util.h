#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H

#include <QPointF>
//#include <QtGui/QOpenGLFunctions>
#include <QGLFunctions>
#include <math.h>

#include "imagegrid.h"
#include "gridwindow.h"

static qreal r2d( qreal r);
static qreal d2r( qreal d );
static QPointF sm2gl( QPointF pixPos, ImageGrid *grid );
static QPointF gl2sm( QPointF glPos, ImageGrid *grid );
static qint64 tileIndex(qint64 row, qint64 col, qint64 cols);


// zoom range & init
const GLfloat m_maxZ = -0.01;
const GLfloat m_minZ = -100.0;
const GLfloat m_initZ = -70.0;
const GLfloat m_stepZ = 5.0;

const GLfloat m_initBrightness = 0.0;
const GLfloat m_initContrast = 100.0;
const GLfloat m_initGamma = 50.0;

const GLfloat m_minBrightness = -100.0;
const GLfloat m_maxBrightness = 100.0;
const GLfloat m_minContrast   = 0.0;
const GLfloat m_maxContrast   = 200.0;
const GLfloat m_minGamma      = 0.0;
const GLfloat m_maxGamma      = 100.0;

const GLfloat m_pi = 3.14159265359;

qreal r2d( qreal r ){
    return r * 57.2957795;
}
qreal d2r( qreal d ){
    return d / 57.2957795;
}

qint64 tileIndex( qint64 row, qint64 col, qint64 cols ){
    qint64 index = cols * row + col;
    return index;
}

//qreal unitToPx(GridWindow *window, ImageGrid *grid){
//    qreal val = 1.0; // Default is something crazy
//    if(grid){
//        qreal dx = qAbs( window->zoom() / window->ruler());
//        qreal dim = grid->dimension();
//        val = dim * dx;
//    }
//    return val;
//}

// Convert surgimap pixel position to gl position
QPointF sm2gl( QPointF pixPos, ImageGrid *grid ){

//    Q_ASSERT(false);
    float x    = pixPos.x();
    float y    = pixPos.y();
    float cols = grid->cols();
    float rows = grid->rows();
    float dim  = grid->dimension();

    x  = x / dim;
    y  = y / dim;

    x = -(cols / 2.0) + x;

    y = (rows / 2.0) - y;

    return QPointF( x, y );
}

// Convert gl coord position to surgimap pixel position
QPointF gl2sm( QPointF glPos, ImageGrid *grid ){

    float x    = glPos.x();
    float y    = glPos.y();
    float cols = grid->cols();
    float rows = grid->rows();
    float dim  = grid->dimension();

    if( y > 0 ){
        y = (( rows / 2.0) - y );
        y *= dim;
    }else{
        y = fabs(y) + (rows / 2.0);
        y *= dim;
    }

    if( x < 0 ){
        x = fabs(-(cols / 2.0) - x);
        x *= dim;
    }else{
        x = x + (cols / 2.0);
        x *= dim;
    }

    return QPointF( x, y );
}

struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};


//// Grid shaders
static const char *vertexShaderSourceG =
        "attribute vec2 aVertexPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "attribute vec4 aColor;\n"
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "varying vec2 vTextureCoord;\n"
        "void main(void) {\n"
        "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 0.0, 1.0);\n"
        "    vTextureCoord = aTextureCoord;\n"
        "}\n";

//        "precision mediump float;\n"
static const char *fragmentShaderSourceG =
        "varying vec2 vTextureCoord;\n"
        "uniform int uInvert;\n"
        "uniform sampler2D uSampler;\n"
        "uniform vec4 uBCG;\n"
        "void main(void) {\n"
        "  vec4 oColor;\n"
        "  oColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));\n"
        "  mediump float brightness = uBCG.x;\n"
        "  mediump float contrast = uBCG.y;\n"
        "  mediump float gamma = uBCG.z;\n"
        "  mediump float b = brightness / 100.0;\n"
        "  mediump float c = contrast / 100.0;\n"
        "  mediump float g;\n"
        "  if (gamma > 50.0) {\n"
        "      g = 1.0 + (gamma - 50.0) / 10.0;\n"
        "  } else {\n"
        "      g = 1.0 / (1.0 + (50.0 - gamma) / 10.0);\n"
        "  }\n"
        "  if( oColor.x == 0.0 && oColor.y == 0.0 && oColor.z == 0.0 ){\n"
        "      oColor.w = 0.0;\n"
        "      oColor.a = 0.0;\n"
        "  }\n"
        "  mediump float bias = (1.0 - c) / 2.0 + b * c;\n"
        "  oColor.x = (pow(((oColor.x * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  oColor.y = (pow(((oColor.y * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  oColor.z = (pow(((oColor.z * 256.0)  * c + 255.0 * bias) / 255.0, 1.0 / g) * 255.0) / 256.0;\n"
        "  if(uInvert > 0){\n"
        "      oColor.x = 1.0 - oColor.x;\n"
        "      oColor.y = 1.0 - oColor.y;\n"
        "      oColor.z = 1.0 - oColor.z;\n"
        "  }\n"
        "  if(oColor.w == 0.0){\n"
        "      oColor.x = 0.0;\n"
        "      oColor.y = 0.0;\n"
        "      oColor.z = 0.0;\n"
        "      oColor.w = 0.0;\n"
        "  }\n"
        "  gl_FragColor = oColor;\n"
        "}\n";

/// Hud text shaders
static const char *vertexShaderSourceT =
        "uniform mat4 uMVMatrix;\n"
        "uniform mat4 uPMatrix;\n"
        "attribute vec4 aVertexPosition;\n"
        "varying vec2 vTextureCoord;\n"
        "void main(void) {\n"
        "   gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition.xy, 0, 1);\n"
        "   vTextureCoord = aVertexPosition.zw;\n"
        "}\n";

static const char *fragmentShaderSourceT =
        "varying vec2 vTextureCoord;\n"
        "uniform vec4 uColor;\n"
        "uniform sampler2D uSampler;\n"
        "void main() {\n"
        "   gl_FragColor = vec4( 1,1,1, texture2D( uSampler, vTextureCoord ).a) * uColor;\n"
        "}\n";

/// Measurement shaders
static const char *vertexShaderSourceM =
        "attribute highp vec4 aVertexPosition;\n"
        "attribute lowp vec4 aColor;\n"
        "varying lowp vec4 vColor;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   vColor = aColor;\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";

static const char *fragmentShaderSourceM =
        "varying lowp vec4 vColor;\n"
        "void main() {\n"
        "   gl_FragColor = vColor;\n"
        "}\n";

/// Stencil shader
static const char *vertexShaderSourceS =
        "attribute highp vec4 aVertexPosition;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";

/// Hud shaders
static const char *vertexShaderSourceH =
        "attribute highp vec4 aVertexPosition;\n"
        "attribute lowp vec4 aColor;\n"
        "varying lowp vec4 vColor;\n"
        "uniform highp mat4 uMVMatrix;\n"
        "uniform highp mat4 uPMatrix;\n"
        "void main() {\n"
        "   vColor = aColor;\n"
        "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
        "}\n";
static const char *fragmentShaderSourceH =
        "varying lowp vec4 vColor;\n"
        "void main() {\n"
        "   gl_FragColor = vColor;\n"
        "}\n";


#endif // GRAPHICS_UTIL_H

