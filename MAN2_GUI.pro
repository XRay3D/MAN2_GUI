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


TARGET = MAN2_GUI
TEMPLATE = app

#win32:RC_FILE = myapp.rc


SOURCES += \
    auto/automaticmeasurements.cpp \
    communications.cpp \
    graduation.cpp \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    hw/man2.cpp \
    hw/myprotokol.cpp \
    hw/scpi.cpp \
    mydialog.cpp \
    shdocvw.cpp \
    auto/worker.cpp \
    hw/interface.cpp \
    preparation\preparation.cpp \
    preparation/devicemodel.cpp \
    preparation/prepareform.cpp \
    preparation/sernummodel.cpp \
    auto/mandatamodel.cpp \
    auto/mesuremodel.cpp


HEADERS  += \
    auto/automaticmeasurements.h \
    communications.h \
    graduation.h \
    mainwindow.h \
    measurements.h \
    hw/common_interfaces.h \
    hw/common_interfaces.h \
    hw/man2.h \
    hw/myprotokol.h \
    hw/scpi.h \
    mydialog.h \
    shdocvw.h \
    auto/worker.h \
    hw/interface.h \
    preparation\preparation.h \
    preparation/devicemodel.h \
    preparation/prepareform.h \
    preparation/sernummodel.h \
    auto/mandatamodel.h \
    auto/mesuremodel.h


FORMS    += \
    graduation.ui \
    communications.ui \
    measurements.ui \
    auto/automaticmeasurements.ui \
    mainwindow.ui \
    man.ui \
    preparation\preparation.ui \
    preparation/prepareform.ui

DISTFILES += \
    modify.txt \
    scratch.txt

RESOURCES += \
    res.qrc
