/****************************************************************************
** Meta object code from reading C++ file 'iedtypeanalysor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../iedtypeanalysor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iedtypeanalysor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IEDTypeAnalysor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      41,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_IEDTypeAnalysor[] = {
    "IEDTypeAnalysor\0\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
};

void IEDTypeAnalysor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IEDTypeAnalysor *_t = static_cast<IEDTypeAnalysor *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData IEDTypeAnalysor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IEDTypeAnalysor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_IEDTypeAnalysor,
      qt_meta_data_IEDTypeAnalysor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IEDTypeAnalysor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IEDTypeAnalysor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IEDTypeAnalysor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IEDTypeAnalysor))
        return static_cast<void*>(const_cast< IEDTypeAnalysor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int IEDTypeAnalysor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
