#-------------------------------------------------
#
# Project created by QtCreator 2019-08-01T11:56:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsScene
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

#CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++17 -Werror=return-type -Werror=pedantic -pedantic-errors

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_CXXFLAGS_DEBUG += -O0

SOURCES += \
        BaseResizableRectEllipseItem.cpp \
        MainWindow.cpp \
        MyRandom.cpp \
        MyScene.cpp \
        ResizableEllipseItem.cpp \
        ResizableLinetItem.cpp \
        ResizablePolygonItem.cpp \
        ResizableRectItem.cpp \
        cornergrabber.cpp \
        main.cpp

HEADERS += \
	BaseResizableRectEllipseItem.h \
	MainWindow.h \
	MyRandom.h \
	MyScene.h \
	ResizableEllipseItem.h \
	ResizableLinetItem.h \
	ResizablePolygonItem.h \
	ResizableRectItem.h \
	cornergrabber.h

FORMS += \
	MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	res.qrc
