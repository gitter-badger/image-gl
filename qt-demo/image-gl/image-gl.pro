#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T10:08:19
#
#-------------------------------------------------

QT       += core gui opengl bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image-gl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glgraphicsview.cpp \
    model.cpp \
    imagegrid.cpp \
    ngraphicsscene.cpp \
    glgraphicsscene.cpp \
    openglwindow.cpp \
    trianglewindow.cpp \
    gridwindow.cpp \
    imagetile.cpp \
    openglfunctionsdebug.cpp \
    btdevice.cpp \
    fontsheet.cpp

HEADERS  += mainwindow.h \
    glgraphicsview.h \
    model.h \
    point3d.h \
    imagegrid.h \
    ngraphicsscene.h \
    glgraphicsscene.h \
    openglwindow.h \
    trianglewindow.h \
    gridwindow.h \
    imagetile.h \
    openglfunctionsdebug.h \
    btdevice.h \
    fontsheet.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

# Include freetype
INCLUDEPATH += /usr/local/include/freetype2
LIBS += /usr/local/lib/libfreetype.a
