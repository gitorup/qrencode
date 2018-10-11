#-------------------------------------------------
#
# Project created by QtCreator 2018-10-08T14:23:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qrtool
TEMPLATE = app

LIBS += -L$$PWD/qrencode -lqrencode
LIBS += $$PWD/qrencode/libqrencode.dll.a
INCLUDEPATH += $$PWD/qrencode

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
