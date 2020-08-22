QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_gameoflifetest.cpp \
    game.cpp

HEADERS += \
    game.h \
    config.h
