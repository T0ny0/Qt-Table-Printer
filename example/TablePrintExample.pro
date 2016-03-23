#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T18:21:24
#
#-------------------------------------------------

QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ../tableprinter.cpp

HEADERS  += widget.h \
    ../tableprinter.h

FORMS    += widget.ui
