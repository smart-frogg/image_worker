QT += core
QT += gui

CONFIG += c++14

TARGET = images
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    smatrimage.cpp \
    imagemap.cpp \
    filterkernel.cpp \
    octave.cpp \
    gausskernelfactory.cpp \
    separablekernel.cpp \
    piramid.cpp \
    simplekernel.cpp \
    sobelfilter.cpp \
    abstractdetector.cpp \
    moravekdetector.cpp \
    harrisdetector.cpp \
    blobdetector.cpp \
    descriptor.cpp \
    haf.cpp \
    transformer.cpp
HEADERS += \
    smatrimage.h \
    imagemap.h \
    filterkernel.h \
    octave.h \
    gausskernelfactory.h \
    separablekernel.h \
    piramid.h \
    simplekernel.h \
    sobelfilter.h \
    abstractdetector.h \
    moravekdetector.h \
    harrisdetector.h \
    blobdetector.h \
    descriptor.h \
    haf.h \
    transformer.h

unix|win32: LIBS += -L$$PWD/../GnuWin32/lib/ -lgsl
unix|win32: LIBS += -L$$PWD/../GnuWin32/lib/ -lgslcblas

INCLUDEPATH += $$PWD/../GnuWin32/include
DEPENDPATH += $$PWD/../GnuWin32/include

unix|win32-g++: PRE_TARGETDEPS += $$PWD/../GnuWin32/lib/libgsl.a
unix|win32-g++: PRE_TARGETDEPS += $$PWD/../GnuWin32/lib/libgslcblas.a
