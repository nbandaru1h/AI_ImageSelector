/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[330];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "showPreviousImage"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 21), // "showPreviousDirectory"
QT_MOC_LITERAL(4, 52, 13), // "showNextImage"
QT_MOC_LITERAL(5, 66, 17), // "showNextDirectory"
QT_MOC_LITERAL(6, 84, 17), // "openTaggingConfig"
QT_MOC_LITERAL(7, 102, 14), // "addImageToList"
QT_MOC_LITERAL(8, 117, 19), // "removeImageFromList"
QT_MOC_LITERAL(9, 137, 13), // "saveImageList"
QT_MOC_LITERAL(10, 151, 14), // "clearImageList"
QT_MOC_LITERAL(11, 166, 19), // "deleteSelectedImage"
QT_MOC_LITERAL(12, 186, 17), // "moveSelectedImage"
QT_MOC_LITERAL(13, 204, 18), // "copySelectedImages"
QT_MOC_LITERAL(14, 223, 23), // "toggleYoloBoundingBoxes"
QT_MOC_LITERAL(15, 247, 30), // "on_loadNamesFileButton_clicked"
QT_MOC_LITERAL(16, 278, 18), // "openImageDirectory"
QT_MOC_LITERAL(17, 297, 32) // "openCurrentImageFolderInExplorer"

    },
    "MainWindow\0showPreviousImage\0\0"
    "showPreviousDirectory\0showNextImage\0"
    "showNextDirectory\0openTaggingConfig\0"
    "addImageToList\0removeImageFromList\0"
    "saveImageList\0clearImageList\0"
    "deleteSelectedImage\0moveSelectedImage\0"
    "copySelectedImages\0toggleYoloBoundingBoxes\0"
    "on_loadNamesFileButton_clicked\0"
    "openImageDirectory\0openCurrentImageFolderInExplorer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,

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

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->showPreviousImage(); break;
        case 1: _t->showPreviousDirectory(); break;
        case 2: _t->showNextImage(); break;
        case 3: _t->showNextDirectory(); break;
        case 4: _t->openTaggingConfig(); break;
        case 5: _t->addImageToList(); break;
        case 6: _t->removeImageFromList(); break;
        case 7: _t->saveImageList(); break;
        case 8: _t->clearImageList(); break;
        case 9: _t->deleteSelectedImage(); break;
        case 10: _t->moveSelectedImage(); break;
        case 11: _t->copySelectedImages(); break;
        case 12: _t->toggleYoloBoundingBoxes(); break;
        case 13: _t->on_loadNamesFileButton_clicked(); break;
        case 14: _t->openImageDirectory(); break;
        case 15: _t->openCurrentImageFolderInExplorer(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
