# CONFIG += debug_and_release build_all
QT +=xml

HEADERS = MainWindow.h \
    ObjectToolDialog.h \
    ObjectDelegate.h \
    TransferFunctionWidget.h \
    ObjectListModel.h
SOURCES = MainWindow.cpp \
    main.cpp \
    ObjectToolDialog.cpp \
    ObjectDelegate.cpp \
    TransferFunctionWidget.cpp \
    ObjectListModel.cpp
FORMS = MainWindow.ui \
    ObjectToolDialog.ui \
    TransferFunctionWidget.ui

TEMPLATE = app
VERSION = 0.9.7

TYPE = 32
contains( QMAKE_CFLAGS, -m64): TYPE =
contains( QMAKE_LIBDIR_X11, /usr/X11R6/lib64):TYPE =

CONFIG(debug, debug|release) { 
    TARGET = Viewer_d
}
else { 
    TARGET = Viewer
}


# Coin-Qt-Wlz glue libraries
CONFIG(debug, debug|release):LIBS += -lWlzQtCoinGlue_d
else {
    LIBS += -lWlzQtCoinGlue
    INCLUDEPATH *= $$(MA_HOME)/include/WlzQtCoinGlue
}
LIBS *= -L/$(MA_HOME)/lib

# static libraries for Woolz
INCLUDEPATH *= $$(MA_HOME)/include
LIBS *= -L/$(MA_HOME)/lib
LIBS *= -lWlzExtFF \
        -lWlz \
        -lAlc \
        -lAlg \
        -ljpeg \
        -ltiff



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
   include($$(MA_HOME)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
}

macx {
    ICON = icon_mac.png
    QMAKE_INFO_PLIST = Info.plist
    OUTDIR = MacOSX
    QMAKE_POST_LINK = ./mac_deploy
    include($$(MA_HOME)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)
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
DESTDIR = $$OUTDIR/bin
MOC_DIR = $$OUTDIR/moc
UI_DIR = $$OUTDIR/ui
RCC_DIR = $$OUTDIR/rcc

target.path = $$(MA_HOME)/bin
INSTALLS += target

RESOURCES += resource.qrc
