#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T16:39:21
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ContainerApp
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    dialoggenerarinstancia.cpp \
    Pisinger/container.c \
    Pisinger/testcont.c \
    containerloadform.cpp \
    containerproblemgenerator.cpp \
    containerproblemsolver.cpp \
    glcontainerwidget.cpp \
    containerproblemform.cpp \
    containersolutiontablemodel.cpp \
    containerproblemtablemodel.cpp \
    dialoganadircaja.cpp \
    containersolution.cpp \
    boxestableview.cpp \
    containerproblem.cpp \
    algorithmthread.cpp \
    algorithmexecutiondialog.cpp

HEADERS  += mainwindow.h \
    dialoggenerarinstancia.h \
    Pisinger/testcont.h \
    containerloadform.h \
    containerproblem.h \
    containerproblemgenerator.h \
    containerproblemsolver.h \
    containersolution.h \
    glcontainerwidget.h \
    containerproblemform.h \
    containersolutiontablemodel.h \
    containerproblemtablemodel.h \
    dialoganadircaja.h \
    boxestableview.h \
    algorithmthread.h \
    algorithmexecutiondialog.h

FORMS    += mainwindow.ui \
    dialoggenerarinstancia.ui \
    containerloadform.ui \
    containerproblemform.ui \
    dialoganadircaja.ui \
    algorithmexecutiondialog.ui

LIBS += -lglut -lGLU

RESOURCES += \
    Resources.qrc
