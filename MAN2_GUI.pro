#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T10:27:13
#
#-------------------------------------------------

QT += core gui widgets serialport printsupport axcontainer
QT += charts
#FQMAKE_CXXFLAGS += vms

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += "Emu=0"

TARGET = MAN2_GUI
TEMPLATE = app

win32:RC_FILE = main_icon/myapp.rc

#win32:RC_FILE = myapp.rc

SOURCES += \
    auto/automeasure.cpp \
    auto/manmodel.cpp \
    auto/mesuremodel.cpp \
    auto/worker.cpp \
    communications.cpp \
    graduation.cpp \
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


HEADERS  += \
    auto/automeasure.h \
    auto/manmodel.h \
    auto/mesuremodel.h \
    auto/worker.h \
    communications.h \
    graduation.h \
    hw/common_interfaces.h \
    hw/common_interfaces.h \
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


FORMS    += \
    auto/automeasure.ui \
    communications.ui \
    graduation.ui \
    mainwindow.ui \
    measurements.ui \
    preparation/prepareform.ui \

DISTFILES += \
    modify.txt \
    scratch.txt

RESOURCES += \
    res.qrc
