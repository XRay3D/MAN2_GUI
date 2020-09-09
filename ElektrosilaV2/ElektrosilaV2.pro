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
    autoform.cpp \
    communications.cpp \
    devicemodel.cpp \
    dialogeditbp.cpp \
    header.cpp \
    hw/digitalosc.cpp \
    hw/interface.cpp \
    hw/man2.cpp \
    hw/myprotokol.cpp \
    hw/scpi.cpp \
    main.cpp \
    mainwindow.cpp \
    manmodel.cpp \
    mantableview.cpp \
    measurements.cpp \
    mydialog.cpp \
    sernummodel.cpp \
    shdocvw.cpp \
    sntableview.cpp \
    tableview.cpp \
    tester.cpp \
    testmodel.cpp \
    teststableview.cpp \


HEADERS += \
    autoform.h \
    communications.h \
    devicemodel.h \
    dialogeditbp.h \
    header.h \
    hw/common_interfaces.h \
    hw/common_interfaces.h \
    hw/digitalosc.h \
    hw/interface.h \
    hw/man2.h \
    hw/myprotokol.h \
    hw/scpi.h \
    mainwindow.h \
    manmodel.h \
    mantableview.h \
    measurements.h \
    mydialog.h \
    sernummodel.h \
    shdocvw.h \
    sntableview.h \
    tableview.h \
    tester.h \
    testmodel.h \
    teststableview.h \

FORMS += \
    autoform.ui \
    communications.ui \
    dialogeditbp.ui \
    mainwindow.ui \
    measurements.ui \

DISTFILES += \
    modify.txt \
    scratch.txt

RESOURCES += \
    res.qrc
