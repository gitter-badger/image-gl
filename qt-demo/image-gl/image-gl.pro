#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T10:08:19
#
#-------------------------------------------------

QT       += core gui opengl bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image-gl
TEMPLATE = app

#CONFIG += has_bluetooth

CONFIG(has_bluetooth){
  DEFINES += HAS_BLUETOOTH
  QT += bluetooth
}

SOURCES += main.cpp\
    mainwindow.cpp \
    glgraphicsview.cpp \
    imagegrid.cpp \
    glgraphicsscene.cpp \
    openglwindow.cpp \
    trianglewindow.cpp \
    gridwindow.cpp \
    imagetile.cpp \
    openglfunctionsdebug.cpp \
    fontsheet.cpp \
    gridimage.cpp \
    openglcontextview.cpp \
    shaders.cpp \
    gridlayer.cpp \
    seriesviewer.cpp \
    viewer.cpp

HEADERS  += mainwindow.h \
    glgraphicsview.h \
    imagegrid.h \
    glgraphicsscene.h \
    openglwindow.h \
    trianglewindow.h \
    gridwindow.h \
    imagetile.h \
    openglfunctionsdebug.h \
    fontsheet.h \
    gridimage.h \
    openglcontextview.h \
    shaders.h \
    gridlayer.h \
    seriesviewer.h \
    viewer.h


FORMS    += mainwindow.ui \
            seriesviewer.ui \
    viewer.ui

CONFIG(has_bluetooth) {
SOURCES +=     bluetoothwidget.cpp \
    btdevice.cpp
HEADERS +=     bluetoothwidget.h \
    btdevice.h
FORMS   +=     bluetoothwidget.ui
}

RESOURCES += \
    resources.qrc


#TODO: Android does not have freetype available
android {
INCLUDEPATH += /Users/Jon/Developer/freetype-2.5.5/android/include/freetype2
LIBS += /Users/Jon/Developer/freetype-2.5.5/android/lib/libfreetype.a
}

mac {
# Include freetype
INCLUDEPATH += /usr/local/include/freetype2
LIBS += /usr/local/lib/libfreetype.a
}

linux-x86 {
INCLUDEPATH += /usr/include/freetype2
LIBS += -lfreetype
}
