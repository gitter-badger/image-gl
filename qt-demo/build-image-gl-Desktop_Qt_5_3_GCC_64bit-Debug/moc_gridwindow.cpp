/****************************************************************************
** Meta object code from reading C++ file 'gridwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../image-gl/gridwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gridwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GridWindow_t {
    QByteArrayData data[22];
    char stringdata[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GridWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GridWindow_t qt_meta_stringdata_GridWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 5),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 7),
QT_MOC_LITERAL(4, 26, 7),
QT_MOC_LITERAL(5, 34, 8),
QT_MOC_LITERAL(6, 43, 10),
QT_MOC_LITERAL(7, 54, 11),
QT_MOC_LITERAL(8, 66, 6),
QT_MOC_LITERAL(9, 73, 10),
QT_MOC_LITERAL(10, 84, 5),
QT_MOC_LITERAL(11, 90, 5),
QT_MOC_LITERAL(12, 96, 5),
QT_MOC_LITERAL(13, 102, 5),
QT_MOC_LITERAL(14, 108, 13),
QT_MOC_LITERAL(15, 122, 9),
QT_MOC_LITERAL(16, 132, 10),
QT_MOC_LITERAL(17, 143, 6),
QT_MOC_LITERAL(18, 150, 7),
QT_MOC_LITERAL(19, 158, 23),
QT_MOC_LITERAL(20, 182, 3),
QT_MOC_LITERAL(21, 186, 6)
    },
    "GridWindow\0panUp\0\0panDown\0panLeft\0"
    "panRight\0rotateLeft\0rotateRight\0invert\0"
    "handleKeys\0flipH\0flipV\0flipX\0flipY\0"
    "toggleAnimate\0rotLeft90\0rotRight90\0"
    "zoomIn\0zoomOut\0handleLoadedGridTexture\0"
    "row\0column"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GridWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x0a /* Public */,
       3,    0,  105,    2, 0x0a /* Public */,
       4,    0,  106,    2, 0x0a /* Public */,
       5,    0,  107,    2, 0x0a /* Public */,
       6,    0,  108,    2, 0x0a /* Public */,
       7,    0,  109,    2, 0x0a /* Public */,
       8,    0,  110,    2, 0x0a /* Public */,
       9,    0,  111,    2, 0x0a /* Public */,
      10,    0,  112,    2, 0x0a /* Public */,
      11,    0,  113,    2, 0x0a /* Public */,
      12,    0,  114,    2, 0x0a /* Public */,
      13,    0,  115,    2, 0x0a /* Public */,
      14,    0,  116,    2, 0x0a /* Public */,
      15,    0,  117,    2, 0x0a /* Public */,
      16,    0,  118,    2, 0x0a /* Public */,
      17,    0,  119,    2, 0x0a /* Public */,
      18,    0,  120,    2, 0x0a /* Public */,
      19,    2,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   20,   21,

       0        // eod
};

void GridWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GridWindow *_t = static_cast<GridWindow *>(_o);
        switch (_id) {
        case 0: _t->panUp(); break;
        case 1: _t->panDown(); break;
        case 2: _t->panLeft(); break;
        case 3: _t->panRight(); break;
        case 4: _t->rotateLeft(); break;
        case 5: _t->rotateRight(); break;
        case 6: _t->invert(); break;
        case 7: _t->handleKeys(); break;
        case 8: _t->flipH(); break;
        case 9: _t->flipV(); break;
        case 10: _t->flipX(); break;
        case 11: _t->flipY(); break;
        case 12: _t->toggleAnimate(); break;
        case 13: _t->rotLeft90(); break;
        case 14: _t->rotRight90(); break;
        case 15: _t->zoomIn(); break;
        case 16: _t->zoomOut(); break;
        case 17: _t->handleLoadedGridTexture((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject GridWindow::staticMetaObject = {
    { &OpenGLWindow::staticMetaObject, qt_meta_stringdata_GridWindow.data,
      qt_meta_data_GridWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *GridWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GridWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GridWindow.stringdata))
        return static_cast<void*>(const_cast< GridWindow*>(this));
    return OpenGLWindow::qt_metacast(_clname);
}

int GridWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OpenGLWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
