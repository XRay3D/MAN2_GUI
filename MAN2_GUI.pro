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
    measuringinterface/maninterface.cpp \
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
    man2.cpp \
    measurements.cpp \
    shdocvw.cpp

HEADERS  += \
    measuringinterface/elemer.h \
    measuringinterface/irt59xx.h \
    measuringinterface/maninterface.h \
    measuringinterface/measuringinterface.h \
    measuringinterface/myprotokol.h \
    measuringinterface/scpi.h \
    qcustomplot/qcustomplot.h \
    automaticmeasurements.h \
    communications.h \
    graduation.h \
    inputparameters.h \
    mainwindow.h \
    man2.h \
    measurements.h \
    shdocvw.h

FORMS    += \
    inputparameters.ui \
    graduation.ui \
    communications.ui \
    measurements.ui \
    automaticmeasurements.ui \
    mainwindow.ui \
    man2.ui

DISTFILES +=

RESOURCES += \
    res.qrc
