#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T10:00:09
#
#-------------------------------------------------

QT       -= gui

TARGET = IntelligentDesignDLL
TEMPLATE = lib

DEFINES += INTELLIGENTDESIGNDLL_LIBRARY

SOURCES += intelligentdesigndll.cpp \
    IntelligentDesign.cpp \
    function.cpp \
    pictureInfo.cpp \
    EXIF.cpp \
    cameraInfo.cpp \
    texture.cpp

HEADERS += intelligentdesigndll.h\
    IntelligentDesign.h \
    public.h \
    pictureInfo.h \
    function.h \
    EXIF.h \
    cameraInfo.h \
    texture.h

INCLUDEPATH +=   D:\Qt\opencv\include

LIBS +=   D:\Qt\opencv\lib\libopencv_core243.dll.a \
  D:\Qt\opencv\lib\libopencv_highgui243.dll.a \
  D:\Qt\opencv\lib\libopencv_imgproc243.dll.a


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
