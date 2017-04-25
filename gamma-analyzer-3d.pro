
QT += core gui 3dcore 3drender 3dextras positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gamma-analyzer-3d
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += lua/lapi.c \
    lua/lauxlib.c \
    lua/lbaselib.c \
    lua/lbitlib.c \
    lua/lcode.c \
    lua/lcorolib.c \
    lua/lctype.c \
    lua/ldblib.c \
    lua/ldebug.c \
    lua/ldo.c \
    lua/ldump.c \
    lua/lfunc.c \
    lua/lgc.c \
    lua/linit.c \
    lua/liolib.c \
    lua/llex.c \
    lua/lmathlib.c \
    lua/lmem.c \
    lua/loadlib.c \
    lua/lobject.c \
    lua/lopcodes.c \
    lua/loslib.c \
    lua/lparser.c \
    lua/lstate.c \
    lua/lstring.c \
    lua/lstrlib.c \
    lua/ltable.c \
    lua/ltablib.c \
    lua/ltm.c \
    lua/lundump.c \
    lua/lutf8lib.c \
    lua/lvm.c \
    lua/lzio.c \
    main.cpp \
    gammaanalyzer3d.cpp \
    session.cpp \
    spectrum.cpp \
    geo.cpp \
    detector.cpp \
    detectortype.cpp \
    colorspectrum.cpp \
    scene.cpp \
    spectrumentity.cpp \
    gridentity.cpp \
    selectionentity.cpp \
    compassentity.cpp

HEADERS += lua/lapi.h \
    lua/lauxlib.h \
    lua/lcode.h \
    lua/lctype.h \
    lua/ldebug.h \
    lua/ldo.h \
    lua/lfunc.h \
    lua/lgc.h \
    lua/llex.h \
    lua/llimits.h \
    lua/lmem.h \
    lua/lobject.h \
    lua/lopcodes.h \
    lua/lparser.h \
    lua/lprefix.h \
    lua/lstate.h \
    lua/lstring.h \
    lua/ltable.h \
    lua/ltm.h \
    lua/lua.h \
    lua/luaconf.h \
    lua/lualib.h \
    lua/lundump.h \
    lua/lvm.h \
    lua/lzio.h \
    gammaanalyzer3d.h \
    session.h \
    spectrum.h \
    geo.h \
    detector.h \
    detectortype.h \
    exceptions.h \
    colorspectrum.h \
    scene.h \
    spectrumentity.h \
    gridentity.h \
    selectionentity.h \
    compassentity.h

FORMS += gammaanalyzer3d.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
