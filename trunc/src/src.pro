TEMPLATE = app
QT       += xml
FORMS += kuzya.ui \
    gotolinedialog.ui \
    optionsdialog.ui \
    finddialog.ui \
    replacedialog.ui
HEADERS += kuzya.h \
    gotolinedialog.h \
    compiler.h \
    optionsdialog.h \
    finddialog.h \
    replacedialog.h \
    helpbrowser.h \
    translator.h \
    version.h \
    lexerloader.h
SOURCES += kuzya.cpp \
    main.cpp \
    gotolinedialog.cpp \
    compiler.cpp \
    optionsdialog.cpp \
    finddialog.cpp \
    replacedialog.cpp \
    translator.cpp \
    lexerloader.cpp
RESOURCES = images.qrc
TRANSLATIONS = ../resources/translations/kuzya_ua.ts
LIBS += -lqscintilla2
unix { 
    TARGET = ../bin/kuzya
    target.path = /usr/bin
    translations.path = /usr/share/kuzya/translations
    translations.files = ../resources/translations/*.qm
    kuzya_profiles.path = /usr/share/kuzya/profiles
    kuzya_profiles.files = ../profiles/*
    kuzya_doc.path = /usr/share/kuzya/doc
    kuzya_doc.files = ../doc/Kuzya_Help/*
    kuzya_resources.path = /usr/share/kuzya/resources
    kuzya_resources.files = ../resources/*
    kuzya_images.path = /usr/share/kuzya
    kuzya_images.file = ./images/kuzya.png
    INSTALLS += target \
        translations \
        kuzya_profiles \
        kuzya_doc \
        kuzya_resources \
        kuzya_images
}
win32 { 
    TARGET = ../../bin/kuzya
    LIBS += -L../../3rdparty/QScintilla/win32/2.4/
    INCLUDEPATH = ../../3rdparty/QScintilla/win32/2.4/
}
