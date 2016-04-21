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
    containerproblemgenerator.cpp \
    containerproblemsolver.cpp \
    glcontainerwidget.cpp \
    containerproblemform.cpp \
    containersolutiontablemodel.cpp \
    containerproblemtablemodel.cpp \
    containersolution.cpp \
    boxestableview.cpp \
    containerproblem.cpp \
    dialoggenerateproblem.cpp \
    dialogaddbox.cpp \
    boxesorderingtablemodel.cpp \
    workercontainerproblemsolver.cpp

HEADERS  += mainwindow.h \
    Pisinger/testcont.h \
    containerproblem.h \
    containerproblemgenerator.h \
    containerproblemsolver.h \
    containersolution.h \
    glcontainerwidget.h \
    containerproblemform.h \
    containersolutiontablemodel.h \
    containerproblemtablemodel.h \
    boxestableview.h \
    dialoggenerateproblem.h \
    dialogaddbox.h \
    boxesorderingtablemodel.h \
    workercontainerproblemsolver.h

FORMS    += mainwindow.ui \
    containerproblemform.ui \
    dialogaddbox.ui \
    dialoggenerateproblem.ui \
    dialogalgorithmexecution.ui

LIBS += -lglut -lGLU

RESOURCES += \
    Resources.qrc
