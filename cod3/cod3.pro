#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T20:59:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cod2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dictionary.cpp

HEADERS  += mainwindow.h \
    dictionary.h

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11
