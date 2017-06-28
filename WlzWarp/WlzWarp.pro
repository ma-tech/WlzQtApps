BUILD_TYPE = $$(BUILD_TYPE)
isEmpty( BUILD_TYPE ) {
  CONFIG += Release
} else {
  message(Setting CONFIG from environment with $$(BUILD_TYPE))
  CONFIG += $$(BUILD_TYPE)
}

CONFIG += build_all
CONFIG += opengl
CONFIG += openmp
QT += xml

DEFINES += PACKAGE_VERSION=\\\"1.1.3\\\"

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
    WoolzTransform.h \
    SectioningPlaneWidget.h \
    WlzWarpVersion.h

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
    PreferencesDialog.cpp \
    ProjectPropertiesDialog.cpp \
    ObjectViewerController.cpp \
    WarpingWidget.cpp \
    ObjectToolWidget.cpp \
    ViewToolWidget.cpp \
    LandmarkWidget.cpp \
    WoolzTransform.cpp \
    SectioningPlaneWidget.cpp \
    WlzWarpVersion.cpp

FORMS = MainWindow.ui \
    ViewToolDialog.ui \
    ObjectToolDialog.ui \
    LandmarkDialog.ui \
    MeshDialog.ui \
    SegmentationWidget.ui \
    ObjectPropertyWidget.ui \
    TransformWidget.ui \
    ContourISOSurfaceWidget.ui \
    PreferencesDialog.ui \
    ProjectPropertiesDialog.ui \
    WarpingWidget.ui \
    ObjectToolWidget.ui \
    ViewToolWidget.ui \
    LandmarkWidget.ui \
    SectioningPlaneWidget.ui

BUILD_CFLAGS = $$(BUILD_CFLAGS)
isEmpty( BUILD_CFLAGS ) {
  message(Using default CFLAGS (set from environment with BUILD_CFLAGS))
} else {
  message(Setting CFLAGS from environment with $$(BUILD_CFLAGS))
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
  QMAKE_CFLAGS_RELEASE -= -O2
  QMAKE_CXXFLAGS_RELEASE -= -O2
  QMAKE_CFLAGS += $$(BUILD_CFLAGS)
  QMAKE_CXXFLAGS += $$(BUILD_CFLAGS)
}

TEMPLATE = app
TYPE = 
openmp { 
    contains( QMAKE_CC, icc):LIBS *= -openmp # for icc
    else:LIBS *= -fopenmp # for gcc
    message( Using openmp )
}
contains( QMAKE_CFLAGS, -m64):TYPE = 
contains( QMAKE_LIBDIR_X11, /usr/lib64):TYPE = 
isEmpty( TYPE):DEFINES += _64BITWARP
else:DEFINES += _32BITWARP
CONFIG(debug, debug|release):TARGET = WlzWarp_d
else:TARGET = WlzWarp

# Coin-Qt-Wlz glue libraries
CONFIG(debug, debug|release):LIBS *= -lWlzQtCoinGlue_d
else {
    LIBS += -lWlzQtCoinGlue
    INCLUDEPATH *= $$(MA)/include/WlzQtCoinGlue
}

# ###########
# INCLUDEPATH *= $$(MA)/include/WlzQtCoinGlue
LIBS *= -L$$(MA)/lib

# Coin/Qt:
SOQTINC = $$(SOQTINC)
isEmpty( SOQTINC ):SOQTINC = $$system(soqt-config --includedir)
COININC = $$(COININC)
isEmpty( COININC ):COININC = $$system( coin-config --includedir )

SOQTLIB = $$(SOQTLIB)
isEmpty( SOQTLIB):SOQTLIB = $$system( soqt-config --libs )
COINLIB = $$(COINLIB)
isEmpty( COINLIB ):COINLIB = $$system(coin-config --libs)
SOQTLDFLAGS = $$(SOQTLDFLAGS)
isEmpty( SOQTLDFLAGS ):SOQTLDFLAGS = $$system(soqt-config --ldflags)
COINLDFLAGS = $$(COINLDFLAGS)
isEmpty( COINLDFLAGS ):COINLDFLAGS = $$system(coin-config --ldflags)
INCLUDEPATH *= $$SOQTINC \
    $$COININC
LIBS *= -lSimVoleon \
    $$SOQTLIB \
    $$SOQTLDFLAGS \
    $$COINLIB \
    $$COINLDFLAGS

# static libraries for Woolz
INCLUDEPATH *= $$(MA)/include
LIBS *= -L$$(MA)/lib
LIBS *= -lWlzExtFF \
    -lWlz \
    -lAlc \
    -lAlg \
    -lhguDlpList \
    -lbibfile \
    -ljpeg \
    -ltiff \
    -llzma \
    -lniftiio \
    -lznz \
    -lbz2 \
    -lz

draggers.files = draggers/*.iv
draggers.path = $$(MA)/bin/draggers

# must precede win32 otherwise cross-compiling causes problems (both win32 and unix are defined)
unix { 
    OUTDIR = linux$$TYPE
    include($$(MA)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
    INSTALLS += draggers
}
macx { 
    ICON = icon_mac.png
    include($$(MA)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
    QMAKE_INFO_PLIST = Info.plist
    OUTDIR = MacOSX
    QMAKE_POST_LINK = ./mac_deploy
}
win32 { 
    include(../../External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
    CONFIG += staticlib
    OUTDIR = win
    DEFINES += COIN_NOT_DLL \
        SOQT_NOT_DLL \
        SIMVOLEON_NOT_DLL
    RC_FILE = windows.rc
}
contains( QMAKE_CC, icc):OUTDIR = $${OUTDIR}_icc
CONFIG(debug, debug|release):OUTDIR = $${OUTDIR}_debug
CONFIG(debug, debug|release) { 
    INCLUDEPATH *= ../WlzQtCoinGlue
    LIBS = -L../WlzQtCoinGlue/$${OUTDIR}/bin \
        $${LIBS}
    PRE_TARGETDEPS += ../WlzQtCoinGlue/$${OUTDIR}/bin/libWlzQtCoinGlue_d.a
}
message( Output directory $$OUTDIR)
OBJECTS_DIR = $$OUTDIR
DESTDIR     = $$OUTDIR/bin
MOC_DIR     = moc
UI_DIR      = ui
RCC_DIR     = $$OUTDIR/rcc

INCLUDEPATH *= ../WlzQtCoinGlue/ui

# install
RESOURCES += resource.qrc
target.path = $$(MA)/bin
INSTALLS += target
OTHER_FILES += openmp.prf
