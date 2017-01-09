QT   += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

unix:QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

HEADERS += readstdin.h \
    graphics.h \
    sleep.h

SOURCES += main.cpp \
    readstdin.cpp \
    graphics.cpp

FORMS += graphics.ui

win32::TARGET = ../../bin/kuzyagraph

unix {
    TARGET = ../bin/kuzyagraph

    target.path = /usr/bin
    kuzya_fpc_graph.path = /usr/include/kuzya/fpc
    kuzya_fpc_graph.files += ./fpc/unit/*
    kuzya_c_graph.path = /usr/include
    kuzya_c_graph.files += ./c/graphics.h

    INSTALLS += target \
        kuzya_fpc_graph \
        kuzya_c_graph
}

RESOURCES += images.qrc
