#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T18:59:20
#
#-------------------------------------------------

QT       += core gui serialport opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = orientation
TEMPLATE = app

isEqual(QT_MAJOR_VERSION, 5) {
    CONFIG += c++11
}

include(src/src.pri)

