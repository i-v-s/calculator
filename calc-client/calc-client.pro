#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T08:32:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calc-client
TEMPLATE = app


SOURCES += main.cpp\
        calcwindow.cpp \
    calcclient.cpp

HEADERS  += calcwindow.h \
    calcclient.h

FORMS    += calcwindow.ui
