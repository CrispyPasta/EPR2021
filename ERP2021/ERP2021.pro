#-------------------------------------------------
#
# Project created by QtCreator 2021-11-03T17:36:52
#
#-------------------------------------------------

QT       += core gui
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ERP2021
TEMPLATE = app

# OpenCV
INCLUDEPATH += /usr/local/include/opencv4
LIBS += `pkg-config --cflags --libs opencv4`

CONFIG += link_pkgconfig

PKGCONFIG += opencv4

QMAKE_CXXFLAGS += -pthread -lJetsonGPIO

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
        mainwindow.cpp \
    Color/Color.cpp \
    Edges/Edges.cpp \
    Matrix/Matrix.cpp \
    Mixing/Mixing.cpp \
    JetsonGPIO/src/gpio_event.cpp \
    JetsonGPIO/src/gpio_pin_data.cpp \
    JetsonGPIO/src/JetsonGPIO.cpp \
    JetsonGPIO/src/PythonFunctions.cpp \
    PID/PID.cpp \

HEADERS += \
        mainwindow.h \
    Color/Color.h \
    Edges/Edges.h \
    Matrix/Matrix.h \
    Mixing/Mixing.h \
    Utility/Utility.h \
    JetsonGPIO/include/private/gpio_event.h \
    JetsonGPIO/include/private/gpio_pin_data.h \
    JetsonGPIO/include/private/Model.h \
    JetsonGPIO/include/private/PythonFunctions.h \
    JetsonGPIO/include/JetsonGPIO.h \
    Camera/Camera.h \
    PID/PID.h \
    Utility/worker.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    Images/edgeOutput.jpg \
    Images/edgeOutput1.jpg \
    Images/edgeOutput2.jpg \
    Images/expandtest.jpg \
    Images/neptune.jpg \
    Images/photo.jpg \
    Images/photoEdges1.jpg \
    Images/rebuilt.jpg \
    Images/snep.jpg \
    Images/snepEdges0.jpg \
    Images/snepEdges1.jpg \
    Images/tinier.jpg \
    Images/tinierEdge.jpg \
    Images/tiny.jpg \
    OutputImages/1.jpg \
    OutputImages/10.jpg \
    OutputImages/11.jpg \
    OutputImages/12.jpg \
    OutputImages/13.jpg \
    OutputImages/14.jpg \
    OutputImages/15.jpg \
    OutputImages/16.jpg \
    OutputImages/17.jpg \
    OutputImages/18.jpg \
    OutputImages/19.jpg \
    OutputImages/2.jpg \
    OutputImages/20.jpg \
    OutputImages/21.jpg \
    OutputImages/22.jpg \
    OutputImages/23.jpg \
    OutputImages/24.jpg \
    OutputImages/25.jpg \
    OutputImages/26.jpg \
    OutputImages/27.jpg \
    OutputImages/28.jpg \
    OutputImages/29.jpg \
    OutputImages/3.jpg \
    OutputImages/30.jpg \
    OutputImages/31.jpg \
    OutputImages/32.jpg \
    OutputImages/33.jpg \
    OutputImages/34.jpg \
    OutputImages/35.jpg \
    OutputImages/36.jpg \
    OutputImages/37.jpg \
    OutputImages/38.jpg \
    OutputImages/39.jpg \
    OutputImages/4.jpg \
    OutputImages/40.jpg \
    OutputImages/41.jpg \
    OutputImages/42.jpg \
    OutputImages/43.jpg \
    OutputImages/44.jpg \
    OutputImages/45.jpg \
    OutputImages/46.jpg \
    OutputImages/47.jpg \
    OutputImages/48.jpg \
    OutputImages/49.jpg \
    OutputImages/5.jpg \
    OutputImages/50.jpg \
    OutputImages/51.jpg \
    OutputImages/52.jpg \
    OutputImages/53.jpg \
    OutputImages/54.jpg \
    OutputImages/55.jpg \
    OutputImages/56.jpg \
    OutputImages/57.jpg \
    OutputImages/58.jpg \
    OutputImages/59.jpg \
    OutputImages/6.jpg \
    OutputImages/60.jpg \
    OutputImages/7.jpg \
    OutputImages/8.jpg \
    OutputImages/9.jpg \
    PID/PIDparams.txt \
    PID/rpmData.csv
