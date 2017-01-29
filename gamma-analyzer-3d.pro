
QT += core gui datavisualization

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
    gui.cpp

HEADERS += \
    gamman3d.h \
    session.h \
    spectrum.h \
    geo.h \
    gui.h \
    exceptions.h

FORMS +=

RESOURCES += \
    resources.qrc
