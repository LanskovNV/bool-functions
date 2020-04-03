TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/bool_func.cpp \
    src/rdnf.cpp

HEADERS += \
    src/bool_func.h \
    src/rdnf.h

DISTFILES += \
    src/input.txt \
    src/output.txt
