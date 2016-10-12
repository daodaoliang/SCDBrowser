/****************************************************************************
** Meta object code from reading C++ file 'Receivewidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Receivewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Receivewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GooseReceiveWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   19,   19,   19, 0x08,
      73,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GooseReceiveWidget[] = {
    "GooseReceiveWidget\0\0enbleTanFouce(bool)\0"
    "setSelecttion(QTableWidgetItem*)\0"
    "setReverse(QTableWidgetItem*)\0"
};

void GooseReceiveWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GooseReceiveWidget *_t = static_cast<GooseReceiveWidget *>(_o);
        switch (_id) {
        case 0: _t->enbleTanFouce((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setSelecttion((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->setReverse((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GooseReceiveWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GooseReceiveWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GooseReceiveWidget,
      qt_meta_data_GooseReceiveWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GooseReceiveWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GooseReceiveWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GooseReceiveWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GooseReceiveWidget))
        return static_cast<void*>(const_cast< GooseReceiveWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int GooseReceiveWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GooseReceiveWidget::enbleTanFouce(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_SvReceiveWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   16,   16,   16, 0x08,
      69,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SvReceiveWidget[] = {
    "SvReceiveWidget\0\0enbleTanFouce(bool)\0"
    "setSelection(QTableWidgetItem*)\0"
    "setReverse(QTableWidgetItem*)\0"
};

void SvReceiveWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SvReceiveWidget *_t = static_cast<SvReceiveWidget *>(_o);
        switch (_id) {
        case 0: _t->enbleTanFouce((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setSelection((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->setReverse((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SvReceiveWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SvReceiveWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SvReceiveWidget,
      qt_meta_data_SvReceiveWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SvReceiveWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SvReceiveWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SvReceiveWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SvReceiveWidget))
        return static_cast<void*>(const_cast< SvReceiveWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SvReceiveWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SvReceiveWidget::enbleTanFouce(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
