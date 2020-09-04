F#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T10:27:13
#
#-------------------------------------------------

QT += core gui widgets serialport printsupport axcontainer
QT += charts

#FQMAKE_CXXFLAGS += vms

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#TARGET = GGEasyr#GeFtber2Gcode

contains(QT_ARCH, i386) {
    TARGET = $$TARGET"_x32"
    LIBS += \
        -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/" \
        -lvisa32
} else {
    TARGET = $$TARGET"_x64"
    LIBS += \
        -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Lib_x64/msc/" \
        -lnivisa64 \
        -lvisa64
}

#msvc* {
#    LIBS += -lsetupapi -lAdvapi32
#    RC_FILE = myapp.rc
#    TARGET = $$TARGET"_msvc"
#}

DESTDIR = $$_PRO_FILE_PWD_/../bin

TEMPLATE = app

win32:RC_FILE = main_icon/myapp.rc

CONFIG += c++17 \
#    console \

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/include"

SOURCES += \
    auto/automeasure.cpp \
    auto/header.cpp \
    auto/manmodel.cpp \
    auto/tableview.cpp \
    auto/testmodel.cpp \
    auto/worker.cpp \
    communications.cpp \
    hw/digitalosc.cpp \
    hw/interface.cpp \
    hw/man2.cpp \
    hw/myprotokol.cpp \
    hw/scpi.cpp \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    mydialog.cpp \
    preparation/devicemodel.cpp \
    preparation/prepareform.cpp \
    preparation/sernummodel.cpp \
    shdocvw.cpp \


HEADERS += \
    auto/automeasure.h \
    auto/header.h \
    auto/manmodel.h \
    auto/tableview.h \
    auto/testmodel.h \
    auto/worker.h \
    communications.h \
    hw/common_interfaces.h \
    hw/common_interfaces.h \
    hw/digitalosc.h \
    hw/interface.h \
    hw/man2.h \
    hw/myprotokol.h \
    hw/scpi.h \
    mainwindow.h \
    measurements.h \
    mydialog.h \
    preparation/devicemodel.h \
    preparation/prepareform.h \
    preparation/sernummodel.h \
    shdocvw.h \

FORMS += \
    auto/automeasure.ui \
    communications.ui \
    mainwindow.ui \
    measurements.ui \
    preparation/prepareform.ui \

DISTFILES += \
    modify.txt \
    scratch.txt

RESOURCES += \
    res.qrc
