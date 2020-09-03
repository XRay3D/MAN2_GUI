/****************************************************************************
** Meta object code from reading C++ file 'measurements.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ElektrosilaV1/measurements.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'measurements.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Measurements_t {
    QByteArrayData data[25];
    char stringdata0[374];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Measurements_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Measurements_t qt_meta_stringdata_Measurements = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Measurements"
QT_MOC_LITERAL(1, 13, 12), // "StartMeasure"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "ValueType"
QT_MOC_LITERAL(4, 37, 4), // "type"
QT_MOC_LITERAL(5, 42, 7), // "uint8_t"
QT_MOC_LITERAL(6, 50, 7), // "channel"
QT_MOC_LITERAL(7, 58, 28), // "on_cbOsc_currentIndexChanged"
QT_MOC_LITERAL(8, 87, 5), // "index"
QT_MOC_LITERAL(9, 93, 18), // "on_pbStart_clicked"
QT_MOC_LITERAL(10, 112, 7), // "checked"
QT_MOC_LITERAL(11, 120, 25), // "on_sbTimeout_valueChanged"
QT_MOC_LITERAL(12, 146, 4), // "arg1"
QT_MOC_LITERAL(13, 151, 32), // "on_dsbSetCurrentAll_valueChanged"
QT_MOC_LITERAL(14, 184, 23), // "on_pbCurrentAll_clicked"
QT_MOC_LITERAL(15, 208, 21), // "on_pbShortAll_clicked"
QT_MOC_LITERAL(16, 230, 13), // "DsbSetCurrent"
QT_MOC_LITERAL(17, 244, 16), // "PbCurrentClicked"
QT_MOC_LITERAL(18, 261, 14), // "PbShortClicked"
QT_MOC_LITERAL(19, 276, 12), // "PbOscClicked"
QT_MOC_LITERAL(20, 289, 20), // "GetMeasuredValueSlot"
QT_MOC_LITERAL(21, 310, 23), // "QMap<int,MeasuredValue>"
QT_MOC_LITERAL(22, 334, 4), // "list"
QT_MOC_LITERAL(23, 339, 15), // "GbChanneClicked"
QT_MOC_LITERAL(24, 355, 18) // "on_pbClear_clicked"

    },
    "Measurements\0StartMeasure\0\0ValueType\0"
    "type\0uint8_t\0channel\0on_cbOsc_currentIndexChanged\0"
    "index\0on_pbStart_clicked\0checked\0"
    "on_sbTimeout_valueChanged\0arg1\0"
    "on_dsbSetCurrentAll_valueChanged\0"
    "on_pbCurrentAll_clicked\0on_pbShortAll_clicked\0"
    "DsbSetCurrent\0PbCurrentClicked\0"
    "PbShortClicked\0PbOscClicked\0"
    "GetMeasuredValueSlot\0QMap<int,MeasuredValue>\0"
    "list\0GbChanneClicked\0on_pbClear_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Measurements[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x06 /* Public */,
       1,    1,   99,    2, 0x26 /* Public | MethodCloned */,
       1,    0,  102,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       7,    1,  103,    2, 0x08 /* Private */,
       9,    1,  106,    2, 0x08 /* Private */,
      11,    1,  109,    2, 0x08 /* Private */,
      13,    1,  112,    2, 0x08 /* Private */,
      14,    1,  115,    2, 0x08 /* Private */,
      15,    1,  118,    2, 0x08 /* Private */,
      16,    1,  121,    2, 0x08 /* Private */,
      17,    1,  124,    2, 0x08 /* Private */,
      18,    1,  127,    2, 0x08 /* Private */,
      19,    1,  130,    2, 0x08 /* Private */,
      20,    1,  133,    2, 0x08 /* Private */,
      23,    1,  136,    2, 0x08 /* Private */,
      24,    0,  139,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void Measurements::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Measurements *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->StartMeasure((*reinterpret_cast< ValueType(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2]))); break;
        case 1: _t->StartMeasure((*reinterpret_cast< ValueType(*)>(_a[1]))); break;
        case 2: _t->StartMeasure(); break;
        case 3: _t->on_cbOsc_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_pbStart_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_sbTimeout_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_dsbSetCurrentAll_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->on_pbCurrentAll_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_pbShortAll_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->DsbSetCurrent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->PbCurrentClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->PbShortClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->PbOscClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->GetMeasuredValueSlot((*reinterpret_cast< const QMap<int,MeasuredValue>(*)>(_a[1]))); break;
        case 14: _t->GbChanneClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_pbClear_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Measurements::*)(ValueType , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Measurements::StartMeasure)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Measurements::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Measurements.data,
    qt_meta_data_Measurements,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Measurements::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Measurements::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Measurements.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Measurements::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void Measurements::StartMeasure(ValueType _t1, uint8_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
