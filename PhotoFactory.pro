#-------------------------------------------------
#
# Project created by QtCreator 2013-02-17T09:37:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoFactory
TEMPLATE = app


SOURCES += main.cpp \
    TemplateData/CTmpLayerData.cpp \
    TemplateData/CPsdTemplate.cpp \
    PhotoData/CPhotoComposePage.cpp \
    PhotoData/CPhotoComposeLayer.cpp \
    PhotoData/CEditorParameter.cpp \
    InitPage/InitPageWidget.cpp \
    EditPage/EditPageWidget.cpp \
    MainWindow.cpp \
    LayerGraphItem/QTmpLayerGraphItem.cpp \
    LayerGraphItem/QTmpLayerGraphDragItem.cpp \
    LayerGraphItem/QTmpLayerGraphRotateItem.cpp \
    LayerGraphItem/QTmpLayerGraphControlItem.cpp \
    LayerControlItem/QTmpLayerItemLabel.cpp \
    LayerControlItem/QTmpLayerControlItem.cpp \
    PhotoItem/QPhotoListWidget.cpp \
    PhotoItem/QPhotoItemIcon.cpp \
    PhotoItem/QPhotoDragListWidget.cpp \
    OperateItem/QOperateLabel.cpp \
    OperateItem/QTransparentButton.cpp \
    OperateItem/QTransparentSlider.cpp \
    LayerGraphItem/QTmpLayerGraphScaleItem.cpp \
    LayerControlItem/QTmpLayerMaskItemLabel.cpp \
    GraphViewScene/QGraphDragView.cpp \
    GraphViewScene/QGraphDragScene.cpp \
    GraphViewScene/QGraphMaskScene.cpp \
    LayerGraphItem/QMaskLayerGraphItem.cpp \
    DrawToolsWidget/QDrawToolsWidget.cpp \
    DrawToolsWidget/QDrawToolsPenWidget.cpp \
    DrawToolsWidget/QDrawToolsBrushWidget.cpp \
    DrawToolsWidget/QDrawToolsIconLabel.cpp \
    EditPage/QTmpLayerListWidget.cpp

HEADERS  += \
    TemplateData/CTmpLayerData.h \
    TemplateData/CPsdTemplate.h \
    PhotoData/CPhotoComposePage.h \
    PhotoData/CPhotoComposeLayer.h \
    PhotoData/CEditorParameter.h \
    InitPage/InitPageWidget.h \
    EditPage/EditPageWidget.h \
    MainWindow.h \
    LayerGraphItem/QTmpLayerGraphItem.h \
    LayerGraphItem/QTmpLayerGraphDragItem.h \
    LayerGraphItem/QTmpLayerGraphRotateItem.h \
    LayerGraphItem/QTmpLayerGraphControlItem.h \
    LayerControlItem/QTmpLayerItemLabel.h \
    LayerControlItem/QTmpLayerControlItem.h \
    PhotoItem/QPhotoListWidget.h \
    PhotoItem/QPhotoItemIcon.h \
    PhotoItem/QPhotoDragListWidget.h \
    OperateItem/QOperateLabel.h \
    OperateItem/QTransparentButton.h \
    OperateItem/QTransparentSlider.h \
    LayerGraphItem/QTmpLayerGraphScaleItem.h \
    LayerControlItem/QTmpLayerMaskItemLabel.h \
    GraphViewScene/QGraphDragView.h \
    GraphViewScene/QGraphDragScene.h \
    GraphViewScene/QGraphMaskScene.h \
    LayerGraphItem/QMaskLayerGraphItem.h \
    DrawToolsWidget/QDrawToolsWidget.h \
    DrawToolsWidget/QDrawToolsPenWidget.h \
    DrawToolsWidget/QDrawToolsBrushWidget.h \
    DrawToolsWidget/QDrawToolsIconLabel.h \
    EditPage/QTmpLayerListWidget.h

FORMS    += MainWindow.ui

RESOURCES += \
    PhotoRes.qrc

INCLUDEPATH += G:\open_source\opencv_src\mingw\include \
                          G:\work_pro\PhotoFactory\template_editor_lib

LIBS += G:\open_source\anormal_lib\mingw\libjpeg.a
LIBS += G:\open_source\anormal_lib\mingw\libpng15.dll.a
LIBS += G:\open_source\anormal_lib\mingw\libzlib.dll.a

LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_core243.dll.a
LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_highgui243.dll.a
LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_video243.dll.a
LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_ml243.dll.a
LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_legacy243.dll.a
LIBS += G:\open_source\opencv_src\mingw\lib\libopencv_imgproc243.dll.a

LIBS += G:\work_pro\PhotoFactory\template_editor_lib\libpsd_template_editor.a
LIBS += G:\work_pro\PhotoFactory\IntelligentDesignDLL\lib\libIntelligentDesignDLL.a

#INCLUDEPATH += G:\open_source\qjson-master\mingw4.4
#LIBS += zlib.lib
#LIBS += jpeg.lib
#LIBS += libpng.lib
#LIBS += qjson.lib
#LIBS += opencv_core243.lib
#LIBS += opencv_highgui243.lib
#LIBS += opencv_video243.lib
#LIBS += opencv_ml243.lib
#LIBS += opencv_legacy243.lib
#LIBS += opencv_imgproc243.lib
#LIBS += template_editor.lib
#LIBS += IntelligentDesignDLL.lib
