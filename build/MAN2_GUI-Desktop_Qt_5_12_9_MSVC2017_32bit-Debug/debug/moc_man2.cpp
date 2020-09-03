/****************************************************************************
** Meta object code from reading C++ file 'man2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ElektrosilaV1/hw/man2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'man2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MAN2_t {
    QByteArrayData data[14];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MAN2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MAN2_t qt_meta_stringdata_MAN2 = {
    {
QT_MOC_LITERAL(0, 0, 4), // "MAN2"
QT_MOC_LITERAL(1, 5, 4), // "Open"
QT_MOC_LITERAL(2, 10, 0), // ""
QT_MOC_LITERAL(3, 11, 4), // "mode"
QT_MOC_LITERAL(4, 16, 5), // "Close"
QT_MOC_LITERAL(5, 22, 5), // "Write"
QT_MOC_LITERAL(6, 28, 4), // "data"
QT_MOC_LITERAL(7, 33, 22), // "GetMeasuredValueSignal"
QT_MOC_LITERAL(8, 56, 23), // "QMap<int,MeasuredValue>"
QT_MOC_LITERAL(9, 80, 20), // "GetMeasuredValueSlot"
QT_MOC_LITERAL(10, 101, 9), // "ValueType"
QT_MOC_LITERAL(11, 111, 4), // "type"
QT_MOC_LITERAL(12, 116, 7), // "uint8_t"
QT_MOC_LITERAL(13, 124, 7) // "channel"

    },
    "MAN2\0Open\0\0mode\0Close\0Write\0data\0"
    "GetMeasuredValueSignal\0QMap<int,MeasuredValue>\0"
    "GetMeasuredValueSlot\0ValueType\0type\0"
    "uint8_t\0channel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MAN2[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    0,   52,    2, 0x06 /* Public */,
       5,    1,   53,    2, 0x06 /* Public */,
       7,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   59,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x2a /* Public | MethodCloned */,
       9,    0,   67,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, 0x80000000 | 8,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 12,   11,   13,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void MAN2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MAN2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Close(); break;
        case 2: _t->Write((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->GetMeasuredValueSignal((*reinterpret_cast< const QMap<int,MeasuredValue>(*)>(_a[1]))); break;
        case 4: _t->GetMeasuredValueSlot((*reinterpret_cast< ValueType(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2]))); break;
        case 5: _t->GetMeasuredValueSlot((*reinterpret_cast< ValueType(*)>(_a[1]))); break;
        case 6: _t->GetMeasuredValueSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MAN2::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAN2::Open)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MAN2::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAN2::Close)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MAN2::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAN2::Write)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MAN2::*)(const QMap<int,MeasuredValue> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAN2::GetMeasuredValueSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MAN2::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MAN2.data,
    qt_meta_data_MAN2,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MAN2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MAN2::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MAN2.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "CommonInterfaces"))
        return static_cast< CommonInterfaces*>(this);
    if (!strcmp(_clname, "CallBack"))
        return static_cast< CallBack*>(this);
    return QObject::qt_metacast(_clname);
}

int MAN2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MAN2::Open(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MAN2::Close()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MAN2::Write(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MAN2::GetMeasuredValueSignal(const QMap<int,MeasuredValue> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_SerialPort_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPort_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPort_t qt_meta_stringdata_SerialPort = {
    {
QT_MOC_LITERAL(0, 0, 10) // "SerialPort"

    },
    "SerialPort"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPort[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SerialPort::staticMetaObject = { {
    &QSerialPort::staticMetaObject,
    qt_meta_stringdata_SerialPort.data,
    qt_meta_data_SerialPort,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPort.stringdata0))
        return static_cast<void*>(this);
    return QSerialPort::qt_metacast(_clname);
}

int SerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSerialPort::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
