/****************************************************************************
** Meta object code from reading C++ file 'multiplot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../app/multiplot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultiPlot_t {
    QByteArrayData data[27];
    char stringdata0[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiPlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiPlot_t qt_meta_stringdata_MultiPlot = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MultiPlot"
QT_MOC_LITERAL(1, 10, 14), // "fftRateChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "fps"
QT_MOC_LITERAL(4, 30, 14), // "fftSizeChanged"
QT_MOC_LITERAL(5, 45, 5), // "value"
QT_MOC_LITERAL(6, 51, 10), // "setFftRate"
QT_MOC_LITERAL(7, 62, 10), // "setFftSize"
QT_MOC_LITERAL(8, 73, 11), // "setFftColor"
QT_MOC_LITERAL(9, 85, 5), // "color"
QT_MOC_LITERAL(10, 91, 10), // "setFftFill"
QT_MOC_LITERAL(11, 102, 6), // "enable"
QT_MOC_LITERAL(12, 109, 16), // "setPeakDetection"
QT_MOC_LITERAL(13, 126, 7), // "enabled"
QT_MOC_LITERAL(14, 134, 14), // "setFftPeakHold"
QT_MOC_LITERAL(15, 149, 13), // "setIqFftSplit"
QT_MOC_LITERAL(16, 163, 6), // "pct_wf"
QT_MOC_LITERAL(17, 170, 11), // "setIqFftAvg"
QT_MOC_LITERAL(18, 182, 3), // "avg"
QT_MOC_LITERAL(19, 186, 15), // "setNewFrequency"
QT_MOC_LITERAL(20, 202, 10), // "FftTimeout"
QT_MOC_LITERAL(21, 213, 24), // "on_plotter_newCenterFreq"
QT_MOC_LITERAL(22, 238, 1), // "f"
QT_MOC_LITERAL(23, 240, 12), // "meterTimeout"
QT_MOC_LITERAL(24, 253, 39), // "on_windowTypeSelect_currentIn..."
QT_MOC_LITERAL(25, 293, 4), // "text"
QT_MOC_LITERAL(26, 298, 38) // "on_numBlocksSelect_currentInd..."

    },
    "MultiPlot\0fftRateChanged\0\0fps\0"
    "fftSizeChanged\0value\0setFftRate\0"
    "setFftSize\0setFftColor\0color\0setFftFill\0"
    "enable\0setPeakDetection\0enabled\0"
    "setFftPeakHold\0setIqFftSplit\0pct_wf\0"
    "setIqFftAvg\0avg\0setNewFrequency\0"
    "FftTimeout\0on_plotter_newCenterFreq\0"
    "f\0meterTimeout\0on_windowTypeSelect_currentIndexChanged\0"
    "text\0on_numBlocksSelect_currentIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiPlot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    1,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,  100,    2, 0x0a /* Public */,
       7,    1,  103,    2, 0x0a /* Public */,
       8,    1,  106,    2, 0x0a /* Public */,
      10,    1,  109,    2, 0x0a /* Public */,
      12,    1,  112,    2, 0x0a /* Public */,
      14,    1,  115,    2, 0x0a /* Public */,
      15,    1,  118,    2, 0x0a /* Public */,
      17,    1,  121,    2, 0x0a /* Public */,
      19,    1,  124,    2, 0x0a /* Public */,
      20,    0,  127,    2, 0x08 /* Private */,
      21,    1,  128,    2, 0x08 /* Private */,
      23,    0,  131,    2, 0x08 /* Private */,
      24,    1,  132,    2, 0x08 /* Private */,
      26,    1,  135,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QColor,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   25,

       0        // eod
};

void MultiPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiPlot *_t = static_cast<MultiPlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fftRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->fftSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setFftRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setFftSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setFftColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->setFftFill((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setPeakDetection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setFftPeakHold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setIqFftSplit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setIqFftAvg((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 10: _t->setNewFrequency((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 11: _t->FftTimeout(); break;
        case 12: _t->on_plotter_newCenterFreq((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 13: _t->meterTimeout(); break;
        case 14: _t->on_windowTypeSelect_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->on_numBlocksSelect_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MultiPlot::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultiPlot::fftRateChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MultiPlot::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultiPlot::fftSizeChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MultiPlot::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MultiPlot.data,
      qt_meta_data_MultiPlot,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MultiPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MultiPlot.stringdata0))
        return static_cast<void*>(const_cast< MultiPlot*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MultiPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void MultiPlot::fftRateChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiPlot::fftSizeChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
