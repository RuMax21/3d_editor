#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T17:50:50
#
#-------------------------------------------------

QT       += core opengl openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3d_editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    simpleobject3d.cpp \
    camera3d.cpp \
    skybox.cpp

HEADERS += \
        widget.h \
    simpleobject3d.h \
    transformation.h \
    camera3d.h \
    skybox.h

DISTFILES += \
    fshadernew.fsh

RESOURCES += \
    shaders.qrc

win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += opengl32.lib
}
