QT += core network
QT -= gui

CONFIG += c++14

TARGET = calc-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    calcserver.cpp \
    calcthread.cpp \
    eval.cpp

HEADERS += \
    calcserver.h \
    calcthread.h \
    eval.h
