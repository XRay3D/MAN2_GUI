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


CONFIG(release, debug|release) {
    message(Release)
    DEFINES += "Emu=0"
}

CONFIG(debug, debug|release) {
    message(Debug)
    DEFINES += "Emu=0"
}

DESTDIR = $$_PRO_FILE_PWD_/../bin

#TARGET = MAN2_DBG
TEMPLATE = app

win32:RC_FILE = main_icon/myapp.rc

CONFIG += c++17
#CONFIG += console


SOURCES += \
    communications.cpp \
    graduation.cpp \
    hw/interface.cpp \
    hw/man2.cpp \
    hw/myprotokol.cpp \
    hw/scpi.cpp \
    lintest.cpp \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    mydialog.cpp \
    shdocvw.cpp \
    triptest.cpp

HEADERS  += \
    communications.h \
    graduation.h \
    hw/common_interfaces.h \
    hw/common_interfaces.h \
    hw/interface.h \
    hw/man2.h \
    hw/myprotokol.h \
    hw/scpi.h \
    lintest.h \
    mainwindow.h \
    measurements.h \
    mydialog.h \
    shdocvw.h \
    triptest.h

FORMS    += \
    communications.ui \
    graduation.ui \
    lintest.ui \
    mainwindow.ui \
    measurements.ui \
    triptest.ui

DISTFILES += \
    MAN_V1.1.X/ad7793.c \
    MAN_V1.1.X/ad7793.h \
    MAN_V1.1.X/adc_service.c \
    MAN_V1.1.X/adc_service.h \
    MAN_V1.1.X/cmp2.c \
    MAN_V1.1.X/cmp2.h \
    MAN_V1.1.X/dac_r2r.c \
    MAN_V1.1.X/dac_r2r.h \
    MAN_V1.1.X/ds18b20.c \
    MAN_V1.1.X/ds18b20.h \
    MAN_V1.1.X/epwm1.c \
    MAN_V1.1.X/epwm1.h \
    MAN_V1.1.X/eusart.c \
    MAN_V1.1.X/eusart.h \
    MAN_V1.1.X/ext_int.c \
    MAN_V1.1.X/ext_int.h \
    MAN_V1.1.X/interrupt_manager.c \
    MAN_V1.1.X/interrupt_manager.h \
    MAN_V1.1.X/main.c \
    MAN_V1.1.X/mcc.c \
    MAN_V1.1.X/mcc.h \
    MAN_V1.1.X/memory.c \
    MAN_V1.1.X/memory.h \
    MAN_V1.1.X/my_protocol.c \
    MAN_V1.1.X/my_protocol.h \
    MAN_V1.1.X/pin_manager.c \
    MAN_V1.1.X/pin_manager.h \
    MAN_V1.1.X/spi.c \
    MAN_V1.1.X/spi.h \
    MAN_V1.1.X/tmr0.c \
    MAN_V1.1.X/tmr0.h \
    MAN_V1.1.X/tmr2.c \
    MAN_V1.1.X/tmr2.h \
    MAN_V1.1.X/tmr3.c \
    MAN_V1.1.X/tmr3.h \
    MAN_V1.1.X/variables.c \
    MAN_V1.1.X/variables.h \
    modify.txt \
    scratch.txt \

RESOURCES += \
    res.qrc
