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
    containersolutiontablemodel.cpp \
    containerproblemtablemodel.cpp \
    containersolution.cpp \
    boxestableview.cpp \
    containerproblem.cpp \
    boxesorderingtablemodel.cpp \
    containerxmlparser.cpp \
    containerproblemsolverthread.cpp \
    containerxmlparserexception.cpp

HEADERS  += mainwindow.h \
    Pisinger/testcont.h \
    containerproblem.h \
    containerproblemgenerator.h \
    containerproblemsolver.h \
    containersolution.h \
    glcontainerwidget.h \
    containersolutiontablemodel.h \
    containerproblemtablemodel.h \
    boxestableview.h \
    boxesorderingtablemodel.h \
    containerxmlparser.h \
    containerproblemsolverthread.h \
    containerxmlparserexception.h

FORMS    += mainwindow.ui \
    dialogaddbox.ui \
    dialoggenerateproblem.ui \
    dialogalgorithmexecution.ui \
    dialogabout.ui \
    dialogmeasurementsystem.ui

RESOURCES += \
    Resources.qrc
