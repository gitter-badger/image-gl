#ifndef IMAGEGRID_H
#define IMAGEGRID_H

#include <QList>
#include <QImage>
#include <QStringList>
#include <QObject>

class ImageTile;
class ImageGrid : public QObject
{
    Q_OBJECT
public:
    ImageGrid(const QString &file, const QString &format, const int tileDimension, QObject *parent = NULL);
    ~ImageGrid();

    qint64 rows() const;
    qint64 cols() const;

    bool initTiles(qint64 dim);
    bool writeJSON(QString path);

    const QString format() const;
    const qint64 dimension() const;

    QStringList errors() const;
    QStringList logs() const;

    bool loadTiles();
    bool loadImage(qint64 dim);
    ImageTile *tile(qint64 row, qint64 col);

    bool writeTiles();

    QString filePath();

    void setIndex(qint64 index);

signals:
    void tileImageLoaded(int index, int row, int col);

private:
    void _log(const QString &message);
    void _error(const QString &message);

    bool _writeImage(QImage &img, QPoint pos, int row, int column, int dim);
    const bool _writeJSON(const QJsonDocument &doc);

    QList< ImageTile *> m_gridTiles;

    int m_dimension;
    QString m_format;
    QString m_file;

    qint64 m_index;
    qint64 m_stretchwidth;
    qint64 m_stretchheight;

    qint64 m_rows;
    qint64 m_cols;
    QImage m_image;

    QStringList m_log;
    QStringList m_error;
};

#endif // IMAGEGRID_H
