#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T10:27:13
#
#-------------------------------------------------

QT += core gui
QT += serialport printsupport axcontainer
#QMAKE_CXXFLAGS += vms

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAN2_GUI
TEMPLATE = app


SOURCES += \
    measuringinterface/elemer.cpp \
    measuringinterface/irt59xx.cpp \
    measuringinterface/measuringinterface.cpp \
    measuringinterface/myprotokol.cpp \
    measuringinterface/scpi.cpp \
    qcustomplot/qcustomplot.cpp \
    automaticmeasurements.cpp \
    communications.cpp \
    graduation.cpp \
    inputparameters.cpp \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    shdocvw.cpp \
    man.cpp \
    measuringinterface/common_interfaces.cpp \
    measuringinterface/common_interfaces.cpp \
    measuringinterface/man2.cpp

HEADERS  += \
    measuringinterface/elemer.h \
    measuringinterface/irt59xx.h \
    measuringinterface/measuringinterface.h \
    measuringinterface/myprotokol.h \
    measuringinterface/scpi.h \
    qcustomplot/qcustomplot.h \
    automaticmeasurements.h \
    communications.h \
    graduation.h \
    inputparameters.h \
    mainwindow.h \
    measurements.h \
    shdocvw.h \
    man.h \
    measuringinterface/common_interfaces.h \
    measuringinterface/common_interfaces.h \
    measuringinterface/man2.h

FORMS    += \
    inputparameters.ui \
    graduation.ui \
    communications.ui \
    measurements.ui \
    automaticmeasurements.ui \
    mainwindow.ui \
    man.ui

DISTFILES += \
    modify.txt

RESOURCES += \
    res.qrc
