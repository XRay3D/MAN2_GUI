/****************************************************************************
** Meta object code from reading C++ file 'measurements.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ElektrosilaDev/measurements.h"
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
    QByteArrayData data[17];
    char stringdata0[215];
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
QT_MOC_LITERAL(7, 58, 18), // "on_pbStart_clicked"
QT_MOC_LITERAL(8, 77, 7), // "checked"
QT_MOC_LITERAL(9, 85, 25), // "on_sbTimeout_valueChanged"
QT_MOC_LITERAL(10, 111, 4), // "arg1"
QT_MOC_LITERAL(11, 116, 20), // "GetMeasuredValueSlot"
QT_MOC_LITERAL(12, 137, 13), // "MeasuredValue"
QT_MOC_LITERAL(13, 151, 4), // "list"
QT_MOC_LITERAL(14, 156, 18), // "on_pbClear_clicked"
QT_MOC_LITERAL(15, 175, 33), // "on_cbxValType_currentIndexCha..."
QT_MOC_LITERAL(16, 209, 5) // "index"

    },
    "Measurements\0StartMeasure\0\0ValueType\0"
    "type\0uint8_t\0channel\0on_pbStart_clicked\0"
    "checked\0on_sbTimeout_valueChanged\0"
    "arg1\0GetMeasuredValueSlot\0MeasuredValue\0"
    "list\0on_pbClear_clicked\0"
    "on_cbxValType_currentIndexChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Measurements[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       1,    1,   59,    2, 0x26 /* Public | MethodCloned */,
       1,    0,   62,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   63,    2, 0x08 /* Private */,
       9,    1,   66,    2, 0x08 /* Private */,
      11,    1,   69,    2, 0x08 /* Private */,
      14,    0,   72,    2, 0x08 /* Private */,
      15,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,

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
        case 3: _t->on_pbStart_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_sbTimeout_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->GetMeasuredValueSlot((*reinterpret_cast< const MeasuredValue(*)>(_a[1]))); break;
        case 6: _t->on_pbClear_clicked(); break;
        case 7: _t->on_cbxValType_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
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
