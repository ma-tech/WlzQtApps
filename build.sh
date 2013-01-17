#!/bin/sh
# This script will configure and build the Woolz Qt Applications on
# Linux systems.

set -x

export MA_HOME=/opt/MouseAtlas
#export MA_HOME=$HOME/MouseAtlas/Build

export PATH="$MA_HOME"/bin:$PATH

cd WlzQtCoinGlue
qmake
make
make install
cd ..

cd WlzViewer
qmake
make
cp linux/bin/Viewer "$MA_HOME"/bin/WlzViewer
cd ..

cd WlzWarp
qmake
make
cp linux/bin/Warping "$MA_HOME"/bin/WlzWarp
cd ..
