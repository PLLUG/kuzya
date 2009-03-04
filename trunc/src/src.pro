
FORMS += kuzya.ui \
 gotolinedialog.ui
HEADERS += kuzya.h \
 gotolinedialog.h \
 compiler.h
SOURCES += kuzya.cpp \
           main.cpp \
 gotolinedialog.cpp \
 compiler.cpp
TARGET =../../bin/kuzya
TEMPLATE = app

win32 {
LIBS += -L../../3rdparty/QScintilla/win32/2.3.2/
INCLUDEPATH = ../../3rdparty/QScintilla/win32/2.3.2/
}

LIBS += -lqscintilla2


TRANSLATIONS    = kuzya

RESOURCES   = images.qrc

