#-------------------------------------------------
#
# Project created by QtCreator 2013-05-26T22:44:17
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nonogram-admin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ok_cancel.cpp \
    localizations.cpp \
    insert_text.cpp \
    crossword.cpp

HEADERS  += mainwindow.h \
    ok_cancel.h \
    insert_text.h \
    crossword.h

FORMS    += mainwindow.ui \
    ok_cancel.ui \
    insert_text.ui

OTHER_FILES +=
