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
    sobelfilter.cpp

HEADERS += \
    smatrimage.h \
    imagemap.h \
    filterkernel.h \
    octave.h \
    gausskernelfactory.h \
    separablekernel.h \
    piramid.h \
    simplekernel.h \
    sobelfilter.h
