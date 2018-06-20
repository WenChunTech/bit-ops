#-------------------------------------------------
#
# Project created by QtCreator 2018-05-27T01:59:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_ICONS = register.ico

TARGET = RegViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    bits.cpp \
    bitbutton.cpp \
    bitbuttons.cpp \
    bitelement.cpp \
    bitlineedit.cpp \
    bitlineedits.cpp \
    bitoperate.cpp \
    bitoptions.cpp \
    common.cpp \
    reginfo.cpp \
    regdisplay.cpp

HEADERS += \
    bits.h \
    bitbutton.h \
    bitbuttons.h \
    bitelement.h \
    bitlineedit.h \
    bitlineedits.h \
    bitoperate.h \
    bitoptions.h \
    include/containers.h \
    include/functions.h \
    include/packToken.h \
    include/shunting-yard.h \
    include/shunting-yard-exceptions.h \
    common.h \
    regdisplay.h \
    reginfo.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lcparse
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lcparse
else:unix: LIBS += -L$$PWD/lib/ -lcparse

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/release/libcparse.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/debug/libcparse.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/release/cparse.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/debug/cparse.lib
else:unix: PRE_TARGETDEPS += $$PWD/lib/libcparse.a
