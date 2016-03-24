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
    sendmessage.cpp \
    animationgrid.cpp \
    ledcolors.cpp \
    launchpadcontroller.cpp \
    mainanimation.cpp \
    selectdevice.cpp \
    frame.cpp \
    animation.cpp \
    about.cpp

HEADERS  += \
    sendmessage.h \
    animationgrid.h \
    ledcolors.h \
    launchpadcontroller.h \
    mainanimation.h \
    selectdevice.h \
    frame.h \
    animation.h \
    about.h

FORMS    += \
    sendmessage.ui \
    animationgrid.ui \
    mainanimation.ui \
    selectdevice.ui \
    about.ui

include(QMidi/QMidi.pri)

RESOURCES += \
    resource.qrc \
    qdarkstyle/style.qrc

RC_FILE = resource.rc
