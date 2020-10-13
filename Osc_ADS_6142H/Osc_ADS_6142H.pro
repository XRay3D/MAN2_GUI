TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$_PRO_FILE_PWD_/../bin

TARGET = ADS_6142H

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

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/include"

SOURCES += \
    digitalosc.cpp \
    main.cpp \

HEADERS += \
    digitalosc.h \


##QT -= gui
#QT += network widgets

#CONFIG += c++17
#CONFIG += console
#CONFIG -= app_bundle

#contains(QT_ARCH, i386) {
#    TARGET = $$TARGET"_x32"
#    LIBS += \
#        -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/" \
#        -lvisa32
#} else {
#    TARGET = $$TARGET"_x64"
#    LIBS += \
#        -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Lib_x64/msc/" \
#        -lnivisa64 \
#        -lvisa64
#}

#INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/include"

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#SOURCES += \
#        digitalosc.cpp \
#        main.cpp \
#        server.cpp

#HEADERS += \
#    digitalosc.h \
#    server.h

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

