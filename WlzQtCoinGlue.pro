# CONFIG += debug_and_release build_all
HEADERS = WoolzObject.h \
    TransferFunction.h \
    ObjectListModelAbstract.h \
    Camera2D.h \
    Viewer2D.h \
    Viewer3D.h \
    Viewer2D3D.h \
    View.h \
    ObjectView.h \
    EmptyView.h \
    Mesh3DView.h \
    Mesh2DView.h \
    WoolzFileObject.h \
    ObjectSimpleViewer.h \
    Contour3DView.h \
    Contour2DView.h \
    VolumeView.h \
    ImageView.h \
    WoolzFileObject.h \
    ClipPlaneButton.h \
    QFunctionEditor.h \
    ClipPlaneButtonBiDirection.h \
    BinaryTransferFunction.h
SOURCES = Mesh3DView.cpp \
    TransferFunction.cpp \
    ObjectListModelAbstract.cpp \
    Camera2D.cpp \
    Viewer2D.cpp \
    Viewer3D.cpp \
    Viewer2D3D.cpp \
    View.cpp \
    ObjectView.cpp \
    EmptyView.cpp \
    Mesh2DView.cpp \
    Contour3DView.cpp \
    Contour2DView.cpp \
    VolumeView.cpp \
    ImageView.cpp \
    WoolzFileObject.cpp \
    WoolzObject.cpp \
    ObjectSimpleViewer.cpp \
    ClipPlaneButton.cpp \
    QFunctionEditor.cpp \
    ClipPlaneButtonBiDirection.cpp \
    BinaryTransferFunction.cpp
TEMPLATE = lib
VERSION = 0.9.7
TYPE = 32
contains( QMAKE_CFLAGS, -m64):TYPE = 
QT += xml
CONFIG(debug, debug|release):TARGET = WlzQtCoinGlue_d
else:TARGET = WlzQtCoinGlue

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

# must precede win32 otherwise cross-compiling causes problems (both win32 and unix are defined)
unix { 
    CONFIG += staticlib
    OUTDIR = linux$$TYPE
}
macx { 
    CONFIG -= staticlib
    OUTDIR = MacOSX
}
CONFIG(debug, debug|release):OUTDIR = $${OUTDIR}_debug
message( Output directory $$OUTDIR)
OBJECTS_DIR = $$OUTDIR
DESTDIR = $$OUTDIR/bin
MOC_DIR = $$OUTDIR/moc
UI_DIR = $$OUTDIR/ui
RCC_DIR = $$OUTDIR/rcc

# install
target.path = /$(MA_HOME)/lib
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    *.pro
sources.path = .
headers.files = $$HEADERS
headers.path = /$(MA_HOME)/include/WlzQtCoinGlue
INSTALLS += target \
    headers
