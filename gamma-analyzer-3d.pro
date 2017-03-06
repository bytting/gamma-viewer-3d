
QT += core gui 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gamma-analyzer-3d
TEMPLATE = app

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp\
    gamman3d.cpp \
    spectrum.cpp \
    session.cpp \
    geo.cpp \
    gui.cpp \
    detectortype.cpp \
    detector.cpp

HEADERS += \
    gamman3d.h \
    session.h \
    spectrum.h \
    geo.h \
    gui.h \
    exceptions.h \
    detectortype.h \
    detector.h

FORMS += \
    gamman3d.ui

RESOURCES += \
    resources.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += lua
