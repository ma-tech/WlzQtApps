CONFIG += release build_all
#CONFIG += debug
CONFIG += QtOpenGL

DEFINES += PACKAGE_VERSION=\\\"1.1.0\\\"

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
    ConvHull2DView.h \
    ConvHull3DView.h \
    VolumeView.h \
    ImageView.h \
    WoolzFileObject.h \
    ClipPlaneButton.h \
    QFunctionEditor.h \
    ClipPlaneButtonBiDirection.h \
    BinaryTransferFunction.h \
    WlzQtCoinVersion.h
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
    ConvHull2DView.cpp \
    ConvHull3DView.cpp \
    VolumeView.cpp \
    ImageView.cpp \
    WoolzFileObject.cpp \
    WoolzObject.cpp \
    ObjectSimpleViewer.cpp \
    ClipPlaneButton.cpp \
    QFunctionEditor.cpp \
    ClipPlaneButtonBiDirection.cpp \
    BinaryTransferFunction.cpp \
    WlzQtCoinVersion.cpp

TEMPLATE = lib
TYPE =
contains( QMAKE_CFLAGS, -m64):TYPE = 
contains( QMAKE_LIBDIR_X11, /usr/X11R6/lib64):TYPE =

isEmpty( TYPE) {
  DEFINES+= _64BITWARP
} else {
  DEFINES+= _32BITWARP
}

QT += xml
CONFIG(debug, debug|release):TARGET = WlzQtCoinGlue_d
else:TARGET = WlzQtCoinGlue

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

INCLUDEPATH *= $$SOQTINC \
    $$COININC
LIBS *= -lSimVoleon \
    $$COINLIB \
    $$COINLDFLAGS \
    $$SOQTLIB \
    $$SOQTLDFLAGS

# static libraries for Woolz
INCLUDEPATH *= $$(MA)/include

message($$INCLUDEPATH)
a=$$(MA)
message($$a)

LIBS *= -L$$(MA)/lib
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
win32 { 
    CONFIG += staticlib
    OUTDIR = win
    DEFINES += COIN_NOT_DLL SOQT_NOT_DLL SIMVOLEON_NOT_DLL 
}

contains( QMAKE_CC, icc) {
    OUTDIR = $${OUTDIR}_icc
}

CONFIG(debug, debug|release):OUTDIR = $${OUTDIR}_debug
message( Output directory $$OUTDIR)
OBJECTS_DIR = $$OUTDIR
DESTDIR = $$OUTDIR/bin
MOC_DIR = $$OUTDIR/moc
UI_DIR = $$OUTDIR/ui
RCC_DIR = $$OUTDIR/rcc

# install
target.path = $$(MA)/lib
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    *.pro
sources.path = .
headers.files = ./$$HEADERS
headers.path = $$(MA)/include/WlzQtCoinGlue
win32 { 
  target.path = $$(MA)\lib
}

INSTALLS += target \
    headers
