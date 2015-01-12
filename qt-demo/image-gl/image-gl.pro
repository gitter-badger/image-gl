#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T10:08:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image-gl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glgraphicsview.cpp \
    glgraphicsscene.cpp

HEADERS  += mainwindow.h \
    glgraphicsview.h \
    glgraphicsscene.h

FORMS    += mainwindow.ui
