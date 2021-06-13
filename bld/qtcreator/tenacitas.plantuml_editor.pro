#-------------------------------------------------
#
# Project created by QtCreator 2019-04-15T10:15:27
#
#-------------------------------------------------

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
#CONFIG -= qt
TARGET = tenacitas.plantuml_editor

include (tenacitas.plantuml_editor.pri)

QT += core gui
QT += widgets

SOURCES += \
    $$src_code_dir/findDialog.cpp \
    $$src_code_dir/main.cpp \
    $$src_code_dir/umlEditor.cpp \
    $$src_code_dir/umlDisplayer.cpp \
    $$src_code_dir/cfg.cpp \
    $$src_code_dir/mainWindow.cpp

HEADERS += \
    $$src_code_dir/findDialog.h \
    $$src_code_dir/umlEditor.h \
    $$src_code_dir/umlDisplayer.h \
    $$src_code_dir/cfg.h \
    $$src_code_dir/mainWindow.h \
    ../../src/events.h

FORMS += \
    $$src_code_dir/findDialog.ui \
    $$src_code_dir/mainwindow.ui \
    $$src_code_dir/umlEditor.ui \
    $$src_code_dir/umlDisplayer.ui \
    $$src_code_dir/cfg.ui

RESOURCES += \
    tenacitas.plantuml_editor.qrc

DISTFILES +=
