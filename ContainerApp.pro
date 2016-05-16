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
    containerproblemtablemodel.cpp \
    containersolution.cpp \
    boxestableview.cpp \
    containerproblem.cpp \
    containerxmlparser.cpp \
    containerproblemsolverthread.cpp \
    containerxmlparserexception.cpp \
    containersolutiontablemodel.cpp

HEADERS  += mainwindow.h \
    Pisinger/testcont.h \
    containerproblem.h \
    containerproblemgenerator.h \
    containerproblemsolver.h \
    containersolution.h \
    glcontainerwidget.h \
    containerproblemtablemodel.h \
    boxestableview.h \
    containerxmlparser.h \
    containerproblemsolverthread.h \
    containerxmlparserexception.h \
    containersolutiontablemodel.h

FORMS    += mainwindow.ui \
    dialogaddbox.ui \
    dialoggenerateproblem.ui \
    dialogalgorithmexecution.ui \
    dialogabout.ui \
    dialogmeasurementsystem.ui

RESOURCES += \
    Resources.qrc

win32:LIBS += -lopengl32
