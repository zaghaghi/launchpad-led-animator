#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T23:04:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaunchpadAnimator
TEMPLATE = app


SOURCES += main.cpp \
    ledcolors.cpp \
    launchpadcontroller.cpp \
    mainanimation.cpp \
    frame.cpp \
    animation.cpp \
    about.cpp \
    RtMidi.cpp \
    mainwindow.cpp

HEADERS  += \
    ledcolors.h \
    launchpadcontroller.h \
    mainanimation.h \
    frame.h \
    animation.h \
    about.h \
    RtMidi.h \
    mainwindow.h

FORMS    += \
    mainanimation.ui \
    about.ui \
    mainwindow.ui

#include(QMidi/QMidi.pri)

RESOURCES += \
    resource.qrc \
    qdarkstyle/style.qrc

RC_FILE = resource.rc

win32 {
    LIBS += -lwinmm
    DEFINES += __WINDOWS_MM__
}

