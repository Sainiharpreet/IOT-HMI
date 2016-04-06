#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T16:50:25
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iot_test_code
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mysocket.cpp

HEADERS  += widget.h \
    mysocket.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc
