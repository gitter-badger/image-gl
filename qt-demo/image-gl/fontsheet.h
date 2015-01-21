#ifndef FONTSHEET_H
#define FONTSHEET_H

#include <QString>
#include <QOpenGLFunctions>

class FontSheet
{
public:
    FontSheet(QString fntFile, QString fontImageFile);
    ~FontSheet();

    GLuint texture(QChar c); // Return the positionBuffer that should be drawn for the char

private:
    QString m_fntFile;
    QString m_fontImageFile;
};

#endif // FONTSHEET_H
