#-------------------------------------------------
#
# Project created by QtCreator 2018-11-01T17:07:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
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
        widget.cpp \
    AsyncGetClient.cpp \
    SyncGetClient.cpp \
    AsyncCustomGetClient.cpp \
    TestGet.cpp

HEADERS += \
        widget.h \
    AsyncGetClient.hpp \
    SyncGetClient.hpp \
    AsyncCustomGetClient.h \
    TestGet.h

FORMS += \
        widget.ui

unix: LIBS += -L$$PWD/../../../../usr/local/Cellar/boost/1.67.0_1/lib/ -lboost_system-mt -lboost_thread-mt

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/boost/1.67.0_1/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/boost/1.67.0_1/include
