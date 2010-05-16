HEADERS += readstdin.h \
    graphics.h
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
