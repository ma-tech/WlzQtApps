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
VERSION = 0.9.5

TYPE = 32
contains( QMAKE_CFLAGS, -m64): TYPE =

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
    INCLUDEPATH *= /$(MA_HOME)/include/WlzQtCoinGlue
}
LIBS *= -L/$(MA_HOME)/lib

# static libraries for Woolz
INCLUDEPATH *= /$(MA_HOME)/include
LIBS *= -L/$(MA_HOME)/lib
LIBS *= -lWlzExtFF \
        -lWlz \
        -lAlc \
        -lAlg \
        -ljpeg \
        -ltiff

# Coin/Qt:
SOQTINC = $$system(soqt-config --includedir)
COININC = $$system(coin-config --includedir)

SOQTLIB = $$system(soqt-config --libs)
COINLIB = $$system(coin-config --libs)

SOQTLDFLAGS = $$system(soqt-config --ldflags)
COINLDFLAGS = $$system(coin-config --ldflags)

INCLUDEPATH += $$SOQTINC $$COININC
LIBS += -lSimVoleon $$COINLIB $$COINLDFLAGS $$SOQTLIB $$SOQTLDFLAGS

include(/$(MA_HOME)/src/External/QtColorPicker/qtcolorpicker/src/qtcolorpicker.pri)

# must precede win32 otherwise cross-compiling causes problems (both win32 and u
=
unix {
   OUTDIR  = linux$$TYPE
}

macx {
    ICON = icon_mac.png
    QMAKE_INFO_PLIST = Info.plist
    OUTDIR = MacOSX
    QMAKE_POST_LINK = ./mac_deploy
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

target.path = /$(MA_HOME)/bin
INSTALLS += target

RESOURCES += resource.qrc
