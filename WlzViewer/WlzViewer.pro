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

DEFINES += PACKAGE_VERSION=\\\"1.1.4\\\"

HEADERS = MainWindow.h \
  ObjectToolDialog.h \
  ObjectDelegate.h \
  ObjectListModel.h \
  WlzViewerVersion.h

SOURCES = MainWindow.cpp \
  main.cpp \
  ObjectToolDialog.cpp \
  ObjectDelegate.cpp \
  ObjectListModel.cpp \
  WlzViewerVersion.cpp

FORMS = MainWindow.ui \
  ObjectToolDialog.ui

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
contains( QMAKE_CFLAGS, -m64): TYPE =
contains( QMAKE_LIBDIR_X11, /usr/lib64):TYPE =

CONFIG(debug, debug|release) { 
    TARGET = WlzViewer_d
}
else { 
    TARGET = WlzViewer
}


# Coin-Qt-Wlz glue libraries
CONFIG(debug, debug|release):LIBS *= -lWlzQtCoinGlue_d
else {
    LIBS *= -lWlzQtCoinGlue
    INCLUDEPATH *= $$(MA)/include/WlzQtCoinGlue
}
LIBS *= -L/$(MA)/lib

# static libraries for Woolz
INCLUDEPATH *= $$(MA)/include
LIBS *= -L/$(MA)/lib
LIBS *= -lWlzExtFF \
        -lWlz \
        -lAlc \
        -lAlg \
	-lbibfile \
   	-lhguDlpList \
        -ljpeg \
        -ltiff \
	-llzma \
	-lniftiio \
	-lznz \
	-lcjson \
	-lbz2 \
	-lz



# Coin/Qt:
SOQTINC = $$(SOQTINC)
isEmpty( SOQTINC ) {
  SOQTINC = $$system(soqt-config --includedir)
}

COININC = $$(COININC)
isEmpty( COININC  ) {
  COININC = $$system( coin-config --includedir )
}

SOQTLIB = $$(SOQTLIB)
isEmpty( SOQTLIB) {
  SOQTLIB = $$system( soqt-config --libs )
}

COINLIB = $$(COINLIB)
isEmpty( COINLIB ) {
  COINLIB = $$system(coin-config --libs)
}

SOQTLDFLAGS = $$(SOQTLDFLAGS)
isEmpty( SOQTLDFLAGS ) {
  SOQTLDFLAGS = $$system(soqt-config --ldflags)
}

COINLDFLAGS = $$(COINLDFLAGS)
isEmpty( COINLDFLAGS ) {
  COINLDFLAGS = $$system(coin-config --ldflags)
}

LIBS += -lSimVoleon $$COINLIB $$COINLDFLAGS $$SOQTLIB $$SOQTLDFLAGS

# must precede win32 otherwise cross-compiling causes problems (both win32 and unix are defined)
unix {
   OUTDIR  = linux$$TYPE
   include($$(MA)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
}

macx {
    ICON = icon_mac.png
    QMAKE_INFO_PLIST = Info.plist
    OUTDIR = MacOSX
    QMAKE_POST_LINK = ./mac_deploy
    include($$(MA)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
}
win32 {
    CONFIG += staticlib
    OUTDIR = win
    DEFINES += COIN_NOT_DLL SOQT_NOT_DLL SIMVOLEON_NOT_DLL
    include(m:/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
}

contains( QMAKE_CC, icc) {
    OUTDIR = $${OUTDIR}_icc
}

CONFIG(debug, debug|release) {
  OUTDIR = $${OUTDIR}_debug
}

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

target.path = $$(MA)/bin
INSTALLS += target

RESOURCES += resource.qrc
