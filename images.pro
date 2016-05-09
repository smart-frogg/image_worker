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



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../gsl/lib/release/ -lgsl
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gsl/lib/debug/ -lgsl
#else:unix: LIBS += -L$$PWD/../../gsl/lib/ -lgsl

#INCLUDEPATH += $$PWD/../../gsl/include
#DEPENDPATH += $$PWD/../../gsl/include

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/release/libgsl.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/debug/libgsl.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/release/gsl.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/debug/gsl.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../gsl/lib/libgsl.a

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../gsl/lib/release/ -lgslcblas
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gsl/lib/debug/ -lgslcblas
#else:unix: LIBS += -L$$PWD/../../gsl/lib/ -lgslcblas

#INCLUDEPATH += $$PWD/../../gsl/include
#DEPENDPATH += $$PWD/../../gsl/include

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/release/libgslcblas.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/debug/libgslcblas.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/release/gslcblas.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gsl/lib/debug/gslcblas.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../gsl/lib/libgslcblas.a

unix|win32: LIBS += -L$$PWD/../GnuWin32/lib/ -lgsl

INCLUDEPATH += $$PWD/../GnuWin32/include
DEPENDPATH += $$PWD/../GnuWin32/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../GnuWin32/lib/gsl.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../GnuWin32/lib/libgsl.a
