/****************************************************************************
** Meta object code from reading C++ file 'agc_options.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtgui/agc_options.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'agc_options.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CAgcOptions_t {
    QByteArrayData data[18];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CAgcOptions_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CAgcOptions_t qt_meta_stringdata_CAgcOptions = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CAgcOptions"
QT_MOC_LITERAL(1, 12, 11), // "gainChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "gain"
QT_MOC_LITERAL(4, 30, 16), // "thresholdChanged"
QT_MOC_LITERAL(5, 47, 9), // "threshold"
QT_MOC_LITERAL(6, 57, 12), // "slopeChanged"
QT_MOC_LITERAL(7, 70, 5), // "slope"
QT_MOC_LITERAL(8, 76, 12), // "decayChanged"
QT_MOC_LITERAL(9, 89, 5), // "decay"
QT_MOC_LITERAL(10, 95, 11), // "hangChanged"
QT_MOC_LITERAL(11, 107, 2), // "on"
QT_MOC_LITERAL(12, 110, 26), // "on_gainSlider_valueChanged"
QT_MOC_LITERAL(13, 137, 31), // "on_thresholdSlider_valueChanged"
QT_MOC_LITERAL(14, 169, 27), // "on_slopeSlider_valueChanged"
QT_MOC_LITERAL(15, 197, 27), // "on_decaySlider_valueChanged"
QT_MOC_LITERAL(16, 225, 21), // "on_hangButton_toggled"
QT_MOC_LITERAL(17, 247, 7) // "checked"

    },
    "CAgcOptions\0gainChanged\0\0gain\0"
    "thresholdChanged\0threshold\0slopeChanged\0"
    "slope\0decayChanged\0decay\0hangChanged\0"
    "on\0on_gainSlider_valueChanged\0"
    "on_thresholdSlider_valueChanged\0"
    "on_slopeSlider_valueChanged\0"
    "on_decaySlider_valueChanged\0"
    "on_hangButton_toggled\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CAgcOptions[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    1,   70,    2, 0x06 /* Public */,
       8,    1,   73,    2, 0x06 /* Public */,
      10,    1,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   79,    2, 0x08 /* Private */,
      13,    1,   82,    2, 0x08 /* Private */,
      14,    1,   85,    2, 0x08 /* Private */,
      15,    1,   88,    2, 0x08 /* Private */,
      16,    1,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   17,

       0        // eod
};

void CAgcOptions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAgcOptions *_t = static_cast<CAgcOptions *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gainChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->thresholdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slopeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->decayChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->hangChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_gainSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_thresholdSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_slopeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_decaySlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_hangButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CAgcOptions::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAgcOptions::gainChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CAgcOptions::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAgcOptions::thresholdChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CAgcOptions::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAgcOptions::slopeChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (CAgcOptions::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAgcOptions::decayChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (CAgcOptions::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAgcOptions::hangChanged)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CAgcOptions::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CAgcOptions.data,
      qt_meta_data_CAgcOptions,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CAgcOptions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CAgcOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CAgcOptions.stringdata0))
        return static_cast<void*>(const_cast< CAgcOptions*>(this));
    return QDialog::qt_metacast(_clname);
}

int CAgcOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CAgcOptions::gainChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAgcOptions::thresholdChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CAgcOptions::slopeChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAgcOptions::decayChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAgcOptions::hangChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
