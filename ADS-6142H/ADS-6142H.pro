QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $$PWD/../bin

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hw/digitalosc.cpp \
    hw/hwi.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    hw/digitalosc.h \
    hw/hwi.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

#win32: LIBS += -L$$PWD/VISA/lib -lvisa32
#INCLUDEPATH += $$PWD/VISA/Include
##DEPENDPATH += $$PWD/.

#INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/include"
#win32: LIBS += "-LC:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc" -lvisa32


INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/include"
#LIBS += \
#    -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Lib_x64/msc/" \
#    -lnivisa64 \
#    -lvisa64
LIBS += \
    -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/" \
#    -lnivisa32 \
    -lvisa32


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
