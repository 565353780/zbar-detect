QT       += core gui

win32{
DESTDIR = ../bin_win
}
unix{
DESTDIR = ../bin_linux
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

# OpenCV
INCLUDEPATH += \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv2

LIBS += \
    -L/home/chli/OpenCV/opencv-3.4.0/build/installed/lib \
    -lopencv_highgui \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_video \
    -lopencv_videoio

# ZBar_Detector
SOURCES += \
    ZBar_Detector/Example.cpp \
    ZBar_Detector/Code_Finder.cpp \
    ZBar_Detector/ZBar_Detector.cpp

HEADERS += \
    ZBar_Detector/Example.h \
    ZBar_Detector/Code_Finder.h \
    ZBar_Detector/ZBar_Detector.h

LIBS += \
    -L/usr/local/lib \
    -lzbar

TRANSLATIONS += \
    ZBar_Detector_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
