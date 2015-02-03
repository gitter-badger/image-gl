#ifndef ShaderManager_H
#define ShaderManager_H

#include <QObject>
#include <QColor>

class ShaderManagerPrivate;
class ShaderManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float brightness READ brightness WRITE setBrightness )
    Q_PROPERTY( float contrast READ contrast WRITE setContrast )
    Q_PROPERTY( float gamma READ gamma WRITE setGamma )
public:
    enum Program {
        SceneProgram,
        StencilProgram,
        HudProgram,
        HudTextProgram,
        MeasurementsProgram
    };

    float brightness() const;
    float contrast() const;
    float gamma() const;
    QColor hudColor() const;
    void updateBCG();
    void updateInvert();

    ~ShaderManager();

    static ShaderManager *instance();

    void updateProgramMatrix(ShaderManager::Program program, QMatrix4x4 mvmatrix, QMatrix4x4 pmatrix);
signals:

public slots:
    void setProgramMatrix(ShaderManager::Program program, const QMatrix4x4 &mvmatrix, const QMatrix4x4 &pmatrix);
    void setBCG( float brightness, float contrast, float gamma );
    void setInvert( bool invert );
    void setBrightness(const float arg);
    void setContrast(const float arg);
    void setGamma(const float arg);
    void setHudColor(const QColor arg);

protected:
    ShaderManager(QObject *parent = 0);

private:
    static ShaderManager *m_instance;
    ShaderManagerPrivate *d;
};

#endif // ShaderManager_H
