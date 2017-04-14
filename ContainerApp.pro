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
    glcontainerwidget.cpp \
    containersolution.cpp \
    containerproblem.cpp \
    containerxmlparser.cpp \
    containerproblemsolverthread.cpp \
    containerxmlparserexception.cpp \
    boxesgroupstableview.cpp \
    containergroupstablemodel.cpp \
    boxesgroupstablemodel.cpp \
    solutionboxestablemodel.cpp

HEADERS  += mainwindow.h \
    Pisinger/testcont.h \
    containerproblem.h \
    containerproblemgenerator.h \
    containersolution.h \
    glcontainerwidget.h \
    containerxmlparser.h \
    containerproblemsolverthread.h \
    containerxmlparserexception.h \
    boxesgroupstableview.h \
    containergroupstablemodel.h \
    boxesgroupstablemodel.h \
    solutionboxestablemodel.h

FORMS    += mainwindow.ui \
    dialoggenerateproblem.ui \
    dialogalgorithmexecution.ui \
    dialogabout.ui \
    dialogmeasurementsystem.ui \
    dialogaddgroup.ui

RESOURCES += \
    Resources.qrc

win32:LIBS += -lopengl32
