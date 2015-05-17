#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T17:22:10
#
#-------------------------------------------------

QT       += core gui network
CONFIG += -std=c++11

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KockanapProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    model/gamemodel.cpp \
    service/tcpservice.cpp

HEADERS  += mainwindow.h \
    model/gamemodel.h \
    service/tcpservice.h \
    logic/tokenize.h \
    logic/gamelogic.h

FORMS    += mainwindow.ui
