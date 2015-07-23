/****************************************************************************
** Meta object code from reading C++ file 'plotter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtgui/plotter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CPlotter_t {
    QByteArrayData data[24];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CPlotter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CPlotter_t qt_meta_stringdata_CPlotter = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CPlotter"
QT_MOC_LITERAL(1, 9, 13), // "newCenterFreq"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 1), // "f"
QT_MOC_LITERAL(4, 26, 12), // "newDemodFreq"
QT_MOC_LITERAL(5, 39, 4), // "freq"
QT_MOC_LITERAL(6, 44, 5), // "delta"
QT_MOC_LITERAL(7, 50, 13), // "newLowCutFreq"
QT_MOC_LITERAL(8, 64, 14), // "newHighCutFreq"
QT_MOC_LITERAL(9, 79, 13), // "newFilterFreq"
QT_MOC_LITERAL(10, 93, 3), // "low"
QT_MOC_LITERAL(11, 97, 4), // "high"
QT_MOC_LITERAL(12, 102, 19), // "resetHorizontalZoom"
QT_MOC_LITERAL(13, 122, 16), // "moveToCenterFreq"
QT_MOC_LITERAL(14, 139, 15), // "moveToDemodFreq"
QT_MOC_LITERAL(15, 155, 11), // "setPeakHold"
QT_MOC_LITERAL(16, 167, 7), // "enabled"
QT_MOC_LITERAL(17, 175, 16), // "setPeakDetection"
QT_MOC_LITERAL(18, 192, 1), // "c"
QT_MOC_LITERAL(19, 194, 15), // "setFftPlotColor"
QT_MOC_LITERAL(20, 210, 5), // "color"
QT_MOC_LITERAL(21, 216, 10), // "setFftFill"
QT_MOC_LITERAL(22, 227, 11), // "zoomOnXAxis"
QT_MOC_LITERAL(23, 239, 5) // "level"

    },
    "CPlotter\0newCenterFreq\0\0f\0newDemodFreq\0"
    "freq\0delta\0newLowCutFreq\0newHighCutFreq\0"
    "newFilterFreq\0low\0high\0resetHorizontalZoom\0"
    "moveToCenterFreq\0moveToDemodFreq\0"
    "setPeakHold\0enabled\0setPeakDetection\0"
    "c\0setFftPlotColor\0color\0setFftFill\0"
    "zoomOnXAxis\0level"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPlotter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    2,   82,    2, 0x06 /* Public */,
       7,    1,   87,    2, 0x06 /* Public */,
       8,    1,   90,    2, 0x06 /* Public */,
       9,    2,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   98,    2, 0x0a /* Public */,
      13,    0,   99,    2, 0x0a /* Public */,
      14,    0,  100,    2, 0x0a /* Public */,
      15,    1,  101,    2, 0x0a /* Public */,
      17,    2,  104,    2, 0x0a /* Public */,
      19,    1,  109,    2, 0x0a /* Public */,
      21,    1,  112,    2, 0x0a /* Public */,
      22,    1,  115,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong,    3,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    5,    6,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Bool, QMetaType::Float,   16,   18,
    QMetaType::Void, QMetaType::QColor,   20,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Float,   23,

       0        // eod
};

void CPlotter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPlotter *_t = static_cast<CPlotter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newCenterFreq((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: _t->newDemodFreq((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 2: _t->newLowCutFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->newHighCutFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->newFilterFreq((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->resetHorizontalZoom(); break;
        case 6: _t->moveToCenterFreq(); break;
        case 7: _t->moveToDemodFreq(); break;
        case 8: _t->setPeakHold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setPeakDetection((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 10: _t->setFftPlotColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 11: _t->setFftFill((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->zoomOnXAxis((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CPlotter::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPlotter::newCenterFreq)) {
                *result = 0;
            }
        }
        {
            typedef void (CPlotter::*_t)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPlotter::newDemodFreq)) {
                *result = 1;
            }
        }
        {
            typedef void (CPlotter::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPlotter::newLowCutFreq)) {
                *result = 2;
            }
        }
        {
            typedef void (CPlotter::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPlotter::newHighCutFreq)) {
                *result = 3;
            }
        }
        {
            typedef void (CPlotter::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPlotter::newFilterFreq)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CPlotter::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CPlotter.data,
      qt_meta_data_CPlotter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CPlotter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPlotter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CPlotter.stringdata0))
        return static_cast<void*>(const_cast< CPlotter*>(this));
    return QFrame::qt_metacast(_clname);
}

int CPlotter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CPlotter::newCenterFreq(qint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPlotter::newDemodFreq(qint64 _t1, qint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPlotter::newLowCutFreq(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPlotter::newHighCutFreq(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CPlotter::newFilterFreq(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
