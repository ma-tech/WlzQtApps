#!/bin/sh
# This script will configure and build the Woolz Qt Applications on
# Linux systems.

set -x

export MA_HOME=/opt/MouseAtlas
#export MA_HOME=$HOME/MouseAtlas/Build
#export MA_HOME=$HOME/MouseAtlas/Build/debug

export MA=$MA_HOME
export PATH="$MA_HOME"/bin:$PATH

# Set build type to Debug or release here:
#export BUILD_TYPE='Debug'
export BUILD_TYPE='Release'

# Set compiler flags here:
export BUILD_CFLAGS='-O3'
#export BUILD_CFLAGS='-O3 -mavx2 -flto'

cd WlzQtCoinGlue
qmake
make clean
make
make install
cd ..

cd WlzViewer
qmake
make clean
make
#cp linux/bin/WlzViewer "$MA_HOME"/bin/WlzViewer
cd ..

cd WlzWarp
qmake
make clean
make
#cp linux/bin/WlzWarp "$MA_HOME"/bin/WlzWarp
cd ..
