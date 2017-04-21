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


SOURCES += main.cpp\
    inputparameters.cpp \
    graduation.cpp \
    scpi.cpp \
    maninterface.cpp \
    myprotokol.cpp \
    measurements.cpp \
    communications.cpp \
    automaticmeasurements.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    irt59xx.cpp \
    elemer.cpp \
    settings.cpp \
    shdocvw.cpp

HEADERS  += \
    inputparameters.h \
    graduation.h \
    scpi.h \
    maninterface.h \
    myprotokol.h \
    measurements.h \
    communications.h \
    automaticmeasurements.h \
    qcustomplot.h \
    mainwindow.h \
    irt59xx.h \
    elemer.h \
    settings.h \
    shdocvw.h

FORMS    += \
    inputparameters.ui \
    graduation.ui \
    communications.ui \
    measurements.ui \
    automaticmeasurements.ui \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    res.qrc
