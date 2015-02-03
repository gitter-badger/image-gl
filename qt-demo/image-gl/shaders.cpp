#include "shaders.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>


class ShaderManagerPrivate : protected QOpenGLFunctions {

public:
    ShaderManagerPrivate(ShaderManager *owner);
    ~ShaderManagerPrivate();

    void setHudColor(const QColor arg);
    void setGamma(const float arg);

    void setBrightness(const float arg);
    void setContrast( const float arg);
    void setInvert( const bool arg );

    float gamma() const;
    float brightness() const;
    float contrast() const;
    bool invert() const;

    void setProgramMatrix( ShaderManager::Program program, const QMatrix4x4 &mvmat, const QMatrix4x4 &pmat );
    void setBCG( float brightness, float contrast, float gamma );

    void updateBCG();
    void updateInvert();

private:
    void initShaderPrograms();
    void initShaderAttributesAndUniforms();

    void setStencilMatrixUniforms();
    void setSceneMatrixUniforms();
    void setHudMatrixUniforms();
    void setHudTextUniforms();
    void setMeasurementUniforms();
    void setSceneColorUniforms();

    /// Shader Programs
    QOpenGLShaderProgram *   m_sceneProgram;
    QOpenGLShaderProgram *   m_stencilProgram;
    QOpenGLShaderProgram *   m_hudProgram;
    QOpenGLShaderProgram *   m_hudTextProgram;
    QOpenGLShaderProgram *   m_measurementsProgram;

    /// SCENE
    GLint                    m_sceneVertexPositionAttribute;
    GLint                    m_sceneTextureCoordAttribute;
    GLint                    m_sceneColorAttribute;
    GLint                    m_sceneUInvert;
    GLint                    m_sceneBCGUniform;
    GLint                    m_sceneMVMatrixUniform;
    GLint                    m_scenePMatrixUniform;
    GLint                    m_sceneSamplerUniform;

    /// STENCIL
    GLint                    m_stencilVertexPositionAttribute;
    GLint                    m_stencilMVMatrixUniform;
    GLint                    m_stencilPMatrixUniform;

    /// HUD
    GLint                    m_hudVertexPositionAttribute ;
    GLint                    m_hudColorAttribute;
    GLint                    m_hudPMatrixUniform;
    GLint                    m_hudMVMatrixUniform;

    /// HUD TEXT
    GLint                    m_hudTextVertexPositionAttribute;
    GLint                    m_hudTextColorUniform;
    GLint                    m_hudTextPMatrixUniform;
    GLint                    m_hudTextMVMatrixUniform;
    GLint                    m_hudTextSamplerUniform;
    GLuint                   m_hudTextTextTexture;
    GLuint                   m_hudTextVbo;
    QVector4D                m_hudTextColor;

    /// MEAUREMENT
    GLint                    m_measurementPMatrixUniform;
    GLint                    m_measurementMVMatrixUniform;
    GLint                    m_measurementVertexPositionAttribute;
    GLint                    m_measurementColorAttribute;

    GLfloat                  m_brightness;
    GLfloat                  m_contrast;
    GLfloat                  m_gamma;
    GLint                    m_invert;
    QColor                   m_hudColor;
    QMatrix4x4               m_pMatrix;
    QMatrix4x4               m_mvMatrix;

    ShaderManager *          m_Owner;

    //// Grid shaders
     const char *vertexShaderSourceG =
            "attribute vec2 aVertexPosition;\n"
            "attribute vec2 aTextureCoord;\n"
            "attribute vec4 aColor;\n"
            "uniform mat4 uMVMatrix;\n"
            "uniform mat4 uPMatrix;\n"
            "varying vec2 vTextureCoord;\n"
            "varying vec4 vColor;\n"
            "void main(void) {\n"
            "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 0.0, 1.0);\n"
            "    vTextureCoord = aTextureCoord;\n"
            "    vColor = aColor;\n"
            "}\n";

    //        "precision mediump float;\n"
     const char *fragmentShaderSourceG =
            "varying vec2 vTextureCoord;\n"
            "uniform int uInvert;\n"
            "uniform sampler2D uSampler;\n"
            "uniform vec4 uBCG;\n"
            "varying vec4 vColor;\n"
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
     const char *vertexShaderSourceT =
            "uniform mat4 uMVMatrix;\n"
            "uniform mat4 uPMatrix;\n"
            "attribute vec4 aVertexPosition;\n"
            "varying vec2 vTextureCoord;\n"
            "void main(void) {\n"
            "   gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition.xy, 0, 1);\n"
            "   vTextureCoord = aVertexPosition.zw;\n"
            "}\n";

     const char *fragmentShaderSourceT =
            "varying vec2 vTextureCoord;\n"
            "uniform vec4 uColor;\n"
            "uniform sampler2D uSampler;\n"
            "void main() {\n"
            "   gl_FragColor = vec4( 1,1,1, texture2D( uSampler, vTextureCoord ).a) * uColor;\n"
            "}\n";

    /// Measurement shaders
     const char *vertexShaderSourceM =
            "attribute highp vec4 aVertexPosition;\n"
            "attribute lowp vec4 aColor;\n"
            "varying lowp vec4 vColor;\n"
            "uniform highp mat4 uMVMatrix;\n"
            "uniform highp mat4 uPMatrix;\n"
            "void main() {\n"
            "   vColor = aColor;\n"
            "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
            "}\n";
     const char *fragmentShaderSourceM =
            "varying lowp vec4 vColor;\n"
            "void main() {\n"
            "   gl_FragColor = vColor;\n"
            "}\n";

    /// Stencil shader
     const char *vertexShaderSourceS =
            "attribute highp vec4 aVertexPosition;\n"
            "uniform highp mat4 uMVMatrix;\n"
            "uniform highp mat4 uPMatrix;\n"
            "void main() {\n"
            "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
            "}\n";

    /// Hud shaders
     const char *vertexShaderSourceH =
            "attribute highp vec4 aVertexPosition;\n"
            "attribute lowp vec4 aColor;\n"
            "varying lowp vec4 vColor;\n"
            "uniform highp mat4 uMVMatrix;\n"
            "uniform highp mat4 uPMatrix;\n"
            "void main() {\n"
            "   vColor = aColor;\n"
            "   gl_Position = uPMatrix * uMVMatrix * aVertexPosition;\n"
            "}\n";
     const char *fragmentShaderSourceH =
            "varying lowp vec4 vColor;\n"
            "void main() {\n"
            "   gl_FragColor = vColor;\n"
            "}\n";
};

ShaderManager::ShaderManager(QObject *parent) : QObject(parent),
    d(new ShaderManagerPrivate(this))
{

}

ShaderManager::~ShaderManager()
{
    delete d;
}

ShaderManager *ShaderManager::m_instance = NULL;

ShaderManager *ShaderManager::instance(){
    if( m_instance == NULL ){
        m_instance = new ShaderManager();
    }
    return m_instance;
}

void ShaderManager::setProgramMatrix(ShaderManager::Program program, const QMatrix4x4 &mvmatrix, const QMatrix4x4 &pmatrix)
{
    d->setProgramMatrix( program, mvmatrix, pmatrix );
}

void ShaderManager::updateBCG()
{
    d->updateBCG();
}

void ShaderManager::updateInvert()
{
    d->updateInvert();
}

void ShaderManager::updateProgramMatrix(ShaderManager::Program program, QMatrix4x4 mvmatrix, QMatrix4x4 pmatrix)
{
    d->setProgramMatrix(program, mvmatrix, pmatrix);
}

float ShaderManager::brightness() const
{
    return d->brightness();
}

float ShaderManager::contrast() const
{
    return d->contrast();
}

float ShaderManager::gamma() const
{
    return d->gamma();
}

void ShaderManager::setBCG(float brightness, float contrast, float gamma)
{
    d->setBCG(brightness, contrast, gamma);
}

void ShaderManager::setInvert(bool invert)
{
    d->setInvert( invert );
}

void ShaderManager::setHudColor(const QColor arg)
{
    d->setHudColor(arg);
}

void ShaderManager::setBrightness(const float arg)
{
    d->setGamma(arg);
}

void ShaderManager::setContrast(const float arg)
{
    d->setContrast(arg);
}

void ShaderManager::setGamma(const float arg)
{
    d->setGamma(arg);
}

ShaderManagerPrivate::ShaderManagerPrivate(ShaderManager *owner):
    m_Owner(owner)
{
    initShaderPrograms();
    initShaderAttributesAndUniforms();
}

ShaderManagerPrivate::~ShaderManagerPrivate()
{
    delete m_measurementsProgram;
    delete m_sceneProgram;
    delete m_stencilProgram;
    delete m_hudProgram;
    delete m_hudTextProgram;
}

void ShaderManagerPrivate::setHudColor(const QColor arg)
{
    m_hudColor = arg;
}

void ShaderManagerPrivate::setGamma(const float arg)
{
    m_gamma = arg;
}

void ShaderManagerPrivate::setBrightness(const float arg)
{
    m_brightness = arg;
}

void ShaderManagerPrivate::setContrast(const float arg)
{
    m_contrast = arg;
}

void ShaderManagerPrivate::setInvert(const bool arg)
{
    m_invert = arg;
}

float ShaderManagerPrivate::gamma() const
{
    return m_gamma;
}

float ShaderManagerPrivate::brightness() const
{
    return m_brightness;
}

float ShaderManagerPrivate::contrast() const
{
    return m_contrast;
}

bool ShaderManagerPrivate::invert() const
{
    return m_invert;
}

void ShaderManagerPrivate::setProgramMatrix(ShaderManager::Program program, const QMatrix4x4 &mvmat, const QMatrix4x4 &pmat)
{
    m_pMatrix = pmat;
    m_mvMatrix = mvmat;

    switch(program){
    case ShaderManager::StencilProgram:
        setStencilMatrixUniforms();
        break;
    case ShaderManager::SceneProgram:
        setSceneMatrixUniforms();
        break;
    case ShaderManager::HudProgram:
        setHudMatrixUniforms();
        break;
    case ShaderManager::HudTextProgram:
        setHudTextUniforms();
        break;
    case ShaderManager::MeasurementsProgram:
        setMeasurementUniforms();
        break;
    }
}

void ShaderManagerPrivate::setBCG(float brightness, float contrast, float gamma)
{
    m_brightness = brightness;
    m_contrast = contrast;
    m_gamma = gamma;
}

void ShaderManagerPrivate::initShaderPrograms(){

    ///// SCENE
    m_sceneProgram = new QOpenGLShaderProgram( m_Owner );
    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceG );
    m_sceneProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceG );
    m_sceneProgram->link();

    ///// HUD TEXT
    m_hudTextProgram = new QOpenGLShaderProgram( m_Owner );
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceT );
    m_hudTextProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceT );
    m_hudTextProgram->link();

    ///// HUD
    m_hudProgram = new QOpenGLShaderProgram( m_Owner );
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceH );
    m_hudProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceH );
    m_hudProgram->link();

    ///// MEASUREMENT
    m_measurementsProgram = new QOpenGLShaderProgram( m_Owner );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceM );
    m_measurementsProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,  fragmentShaderSourceM );
    m_measurementsProgram->link();

    ///// STENCIL
    m_stencilProgram = new QOpenGLShaderProgram( m_Owner );
    m_stencilProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,    vertexShaderSourceS );
    m_stencilProgram->link();
}

void ShaderManagerPrivate::initShaderAttributesAndUniforms(){

    ///// SCENE
    m_sceneVertexPositionAttribute         = m_sceneProgram->attributeLocation( "aVertexPosition" );
    m_sceneTextureCoordAttribute           = m_sceneProgram->attributeLocation( "aTextureCoord" );
    m_sceneColorAttribute                  = m_sceneProgram->attributeLocation( "aColor" );
    m_sceneBCGUniform 			           = m_sceneProgram->uniformLocation( "uBCG" );
    m_sceneMVMatrixUniform                 = m_sceneProgram->uniformLocation( "uMVMatrix" );
    m_scenePMatrixUniform                  = m_sceneProgram->uniformLocation( "uPMatrix" );
    m_sceneUInvert 			               = m_sceneProgram->uniformLocation( "uInvert" );
    m_sceneSamplerUniform 	               = m_sceneProgram->uniformLocation( "uSampler" );

    ///// HUD TEXT
    m_hudTextVertexPositionAttribute       = m_hudTextProgram->attributeLocation( "aVertexPosition" );
    m_hudTextColorUniform    			   = m_hudTextProgram->uniformLocation( "uColor" );
    m_hudTextPMatrixUniform                = m_hudTextProgram->uniformLocation( "uPMatrix" );
    m_hudTextMVMatrixUniform               = m_hudTextProgram->uniformLocation( "uMVMatrix" );
    m_hudTextSamplerUniform 	           = m_hudTextProgram->uniformLocation( "uSampler" );

    ///// HUD
    m_hudVertexPositionAttribute           = m_hudProgram->attributeLocation( "aVertexPosition" );
    m_hudColorAttribute 			       = m_hudProgram->attributeLocation( "aColor" );
    m_hudPMatrixUniform                    = m_hudProgram->uniformLocation( "uPMatrix" );
    m_hudMVMatrixUniform                   = m_hudProgram->uniformLocation( "uMVMatrix" );

    ///// MEASUREMENT
    m_measurementVertexPositionAttribute   = m_measurementsProgram->attributeLocation( "aVertexPosition" );
    m_measurementColorAttribute 	       = m_measurementsProgram->attributeLocation( "aColor" );
    m_measurementPMatrixUniform            = m_measurementsProgram->uniformLocation( "uPMatrix" );
    m_measurementMVMatrixUniform           = m_measurementsProgram->uniformLocation( "uMVMatrix" );

    ///// STENCIL
    m_stencilVertexPositionAttribute      = m_stencilProgram->attributeLocation( "aVertexPosition" );
    m_stencilPMatrixUniform               = m_stencilProgram->uniformLocation( "uPMatrix" );
    m_stencilMVMatrixUniform              = m_stencilProgram->uniformLocation( "uMVMatrix" );
}

////////
void ShaderManagerPrivate::setStencilMatrixUniforms(){
    m_stencilProgram->setUniformValue( m_stencilPMatrixUniform, m_pMatrix );
    m_stencilProgram->setUniformValue( m_stencilMVMatrixUniform, m_mvMatrix );
}

void ShaderManagerPrivate::setSceneMatrixUniforms(){
    m_sceneProgram->setUniformValue( m_scenePMatrixUniform, m_pMatrix );
    m_sceneProgram->setUniformValue( m_sceneMVMatrixUniform, m_mvMatrix );
}

void ShaderManagerPrivate::setHudMatrixUniforms(){
    m_hudProgram->setUniformValue( m_hudPMatrixUniform, m_pMatrix );
    m_hudProgram->setUniformValue( m_hudMVMatrixUniform, m_mvMatrix );
}

void ShaderManagerPrivate::setHudTextUniforms(){
    m_hudTextProgram->setUniformValue( m_hudTextPMatrixUniform,  m_pMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextMVMatrixUniform, m_mvMatrix);
    m_hudTextProgram->setUniformValue( m_hudTextColorUniform,    m_hudTextColor);
}

void ShaderManagerPrivate::setMeasurementUniforms(){
    m_measurementsProgram->setUniformValue( m_measurementPMatrixUniform,  m_pMatrix );
    m_measurementsProgram->setUniformValue( m_measurementMVMatrixUniform, m_mvMatrix );
}

void ShaderManagerPrivate::setSceneColorUniforms(){
    updateBCG();
    updateInvert();
}

//// TODO: Merge BCG & Invert uniforms by putting invert into the 4th (where 1.0 is)
void ShaderManagerPrivate::updateBCG() {
    glUniform4f( m_sceneBCGUniform, m_brightness, m_contrast, m_gamma, 1.0 );
}

void ShaderManagerPrivate::updateInvert() {
    glUniform1i( m_sceneUInvert, m_invert == true ? 1 : 0 );
}
