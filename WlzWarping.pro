# CONFIG += debug_and_release build_all
# CONFIG += openmp
CONFIG += sse2
QT += xml
HEADERS = WarperConfig.h \
    WarperController.h \
    SnapSurfaceManip.h \
    SnapSurfaceDragger.h \
    TranslateRadialDragger.h \
    TranslateRadialManip.h \
    CrossHairDragger.h \
    CrossHairManip.h \
    MainWindow.h \
    ProjectProperties.h \
    WarperViewer.h \
    WarperSourceViewer.h \
    WarperTargetViewer.h \
    WarperResultViewer.h \
    ViewToolDialog.h \
    ObjectToolDialog.h \
    LandmarkDialog.h \
    ObjectViewerModel.h \
    ObjectListModel.h \
    LandmarkModel.h \
    ViewerDelegate.h \
    ObjectDelegate.h \
    ViewerListEditor.h \
    LoadViewThread.h \
    ObjectPropertyWidget.h \
    LandmarkView.h \
    LandmarkDragger.h \
    LandmarkManip.h \
    WoolzDynWarpedObject.h \
    WoolzDynObject.h \
    WoolzDynMeshObject.h \
    WoolzDynThresholdedObj.h \
    MeshDialog.h \
    SegmentationWidget.h \
    TransformWidget.h \
    LandmarkController.h \
    Commands.h \
    ContourISOSurfaceWidget.h \
    WoolzDynContourISO.h \
    ObjectViewer.h \
    TransferFunctionWidget.h \
    draggers/CrossHairDraggerGeom.h \
    draggers/TranslateRadialDraggerGeom.h \
    draggers/SnapSurfaceDraggerGeom.h \
    PreferencesDialog.h \
    ProjectPropertiesDialog.h \
    ObjectViewerController.h \
    WarpingWidget.h \
    ObjectToolWidget.h \
    ViewToolWidget.h \
    LandmarkWidget.h \
    WoolzTransform.h
SOURCES = WarperConfig.cpp \
    SnapSurfaceManip.cpp \
    SnapSurfaceDragger.cpp \
    TranslateRadialDragger.cpp \
    TranslateRadialManip.cpp \
    CrossHairDragger.cpp \
    CrossHairManip.cpp \
    MainWindow.cpp \
    ProjectProperties.cpp \
    WarperController.cpp \
    WarperViewer.cpp \
    WarperSourceViewer.cpp \
    WarperTargetViewer.cpp \
    WarperResultViewer.cpp \
    main.cpp \
    ViewToolDialog.cpp \
    ObjectToolDialog.cpp \
    LandmarkDialog.cpp \
    ObjectListModel.cpp \
    ObjectViewerModel.cpp \
    LandmarkModel.cpp \
    ViewerDelegate.cpp \
    ObjectDelegate.cpp \
    ViewerListEditor.cpp \
    LoadViewThread.cpp \
    ObjectPropertyWidget.cpp \
    LandmarkView.cpp \
    LandmarkManip.cpp \
    LandmarkDragger.cpp \
    WoolzDynWarpedObject.cpp \
    WoolzDynThresholdedObj.cpp \
    WoolzDynObject.cpp \
    WoolzDynMeshObject.cpp \
    MeshDialog.cpp \
    SegmentationWidget.cpp \
    TransformWidget.cpp \
    LandmarkController.cpp \
    Commands.cpp \
    ContourISOSurfaceWidget.cpp \
    WoolzDynContourISO.cpp \
    ObjectViewer.cpp \
    TransferFunctionWidget.cpp \
    PreferencesDialog.cpp \
    ProjectPropertiesDialog.cpp \
    ObjectViewerController.cpp \
    WarpingWidget.cpp \
    ObjectToolWidget.cpp \
    ViewToolWidget.cpp \
    LandmarkWidget.cpp \
    WoolzTransform.cpp
FORMS = MainWindow.ui \
    ViewToolDialog.ui \
    ObjectToolDialog.ui \
    LandmarkDialog.ui \
    MeshDialog.ui \
    SegmentationWidget.ui \
    ObjectPropertyWidget.ui \
    TransformWidget.ui \
    ContourISOSurfaceWidget.ui \
    TransferFunctionWidget.ui \
    PreferencesDialog.ui \
    ProjectPropertiesDialog.ui \
    WarpingWidget.ui \
    ObjectToolWidget.ui \
    ViewToolWidget.ui \
    LandmarkWidget.ui
TEMPLATE = app
VERSION = 0.9.7
TYPE = 32

openmp {
  LIBS *= -openmp
  message( Using openmp )
}

contains( QMAKE_CFLAGS, -m64):TYPE = 
contains( QMAKE_LIBDIR_X11, /usr/X11R6/lib64):TYPE =

CONFIG(debug, debug|release):TARGET = Warping_d
else:TARGET = Warping

# Coin-Qt-Wlz glue libraries
CONFIG(debug, debug|release):LIBS += -lWlzQtCoinGlue_d
else { 
    LIBS += -lWlzQtCoinGlue
    INCLUDEPATH *= /$(MA_HOME)/include/WlzQtCoinGlue
}
LIBS *= -L/$(MA_HOME)/lib

# Coin/Qt:
SOQTINC = $$system(soqt-config --includedir)
COININC = $$system(coin-config --includedir)
SOQTLIB = $$system(soqt-config --libs)
COINLIB = $$system(coin-config --libs)
SOQTLDFLAGS = $$system(soqt-config --ldflags)
COINLDFLAGS = $$system(coin-config --ldflags)
INCLUDEPATH *= $$SOQTINC \
    $$COININC
LIBS *= -lSimVoleon \
    $$COINLIB \
    $$COINLDFLAGS \
    $$SOQTLIB \
    $$SOQTLDFLAGS

# static libraries for Woolz
INCLUDEPATH *= /$(MA_HOME)/include
LIBS *= -L/$(MA_HOME)/lib
LIBS *= -lWlzExtFF \
    -lWlz \
    -lAlc \
    -lAlg \
    -ljpeg \
    -ltiff
include(/$(MA_HOME)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
draggers.files = draggers/*.iv
draggers.path = /$(MA_HOME)/bin/draggers

# must precede win32 otherwise cross-compiling causes problems (both win32 and u
unix { 
    OUTDIR = linux$$TYPE
    INSTALLS += draggers
}
macx { 
    ICON = icon_mac.png
    QMAKE_INFO_PLIST = Info.plist
    OUTDIR = MacOSX
    QMAKE_POST_LINK = ./mac_deploy
}
CONFIG(debug, debug|release):OUTDIR = $${OUTDIR}_debug
CONFIG(debug, debug|release) { 
    INCLUDEPATH *= ../WlzQtCoinGlue
    LIBS = -L../WlzQtCoinGlue/$${OUTDIR}/bin \
        $${LIBS}
    PRE_TARGETDEPS += ../WlzQtCoinGlue/$${OUTDIR}/bin/libWlzQtCoinGlue_d.a
}
message( Output directory $$OUTDIR)
OBJECTS_DIR = $$OUTDIR
DESTDIR = $$OUTDIR/bin
MOC_DIR = $$OUTDIR/moc
UI_DIR = $$OUTDIR/ui
RCC_DIR = $$OUTDIR/rcc

# install
RESOURCES += resource.qrc
target.path = /$(MA_HOME)/bin
INSTALLS += target
OTHER_FILES += openmp.prf
