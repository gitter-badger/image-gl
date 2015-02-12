#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QVector>

#include <math.h>

#include "point3d.h"

class Model
{
public:
    Model(const QString &filePath, float vscale = 1.0);

    void render( bool wireframe = false, bool normals = false) const;

    QString fileName() const { return m_fileName; }
    int faces() const { return m_pointIndices.size() / 3; }
    int edges() const { return m_edgeIndices.size() / 2; }
    int points() const { return m_points.size(); }

    QVector<int> edgeIndices(){ return m_edgeIndices; }
    QVector<int> pointIndices() { return m_pointIndices; }

    void setScale(qreal vscale);

private:
    QString m_fileName;
    QVector<Point3d> m_points;
    QVector<Point3d> m_scaledpoints;
    QVector<Point3d> m_normals;
    QVector<int> m_edgeIndices;
    QVector<int> m_pointIndices;

    qreal m_scale;
};

#endif
