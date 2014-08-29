#!/bin/sh
# This script will configure and build the Woolz Qt Applications on
# Linux systems.

set -x

export MA=/opt/MouseAtlas
#setenv MA /opt/MouseAtlas
#export MA=$HOME/MouseAtlas/Build

#set path=("$MA"/bin $path)
export PATH="$MA"/bin:$PATH

cd WlzQtCoinGlue
qmake
make
make install
cd ..

cd WlzViewer
qmake
make
cp linux/bin/Viewer "$MA"/bin/WlzViewer
cd ..

cd WlzWarp
qmake
make
cp linux/bin/Warping "$MA"/bin/WlzWarp
cd ..
