#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T14:18:29
#
#-------------------------------------------------

QT       += core network
HEADER += CONFIG += -std=c++11
QMAKE_CXXFLAGS += -std=c++11



TARGET = KOCKA
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE += app


SOURCES += main.cpp

HEADERS += \
    main.h \
    logic/gamelogic.h \
    logic/tokenize.h \
    logic/save_gamelogic.h \
    logic/gamelogic_pot.h \
    logic/tokenize_pot.h
