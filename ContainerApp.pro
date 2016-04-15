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
    Pisinger/container.c \
    Pisinger/testcont.c \
    containerloadform.cpp \
    containerproblemgenerator.cpp \
    containerproblemsolver.cpp \
    glcontainerwidget.cpp \
    containerproblemform.cpp \
    containersolutiontablemodel.cpp \
    containerproblemtablemodel.cpp \
    containersolution.cpp \
    boxestableview.cpp \
    containerproblem.cpp \
    algorithmthread.cpp \
    algorithmexecutiondialog.cpp \
    dialoggenerateproblem.cpp \
    dialogaddbox.cpp

HEADERS  += mainwindow.h \
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
    boxestableview.h \
    algorithmthread.h \
    algorithmexecutiondialog.h \
    dialoggenerateproblem.h \
    dialogaddbox.h

FORMS    += mainwindow.ui \
    containerloadform.ui \
    containerproblemform.ui \
    algorithmexecutiondialog.ui \
    dialogaddbox.ui \
    dialoggenerateproblem.ui

LIBS += -lglut -lGLU

RESOURCES += \
    Resources.qrc
