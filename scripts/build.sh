#!/bin/bash

BUILD_DIR=build

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUNDLE_LIBGIT2=ON ..

make -j

DESTDIR=install make install

rm -rf install/usr/include

wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
# export VERSION=... # linuxdeployqt uses this for naming the file
./linuxdeployqt-continuous-x86_64.AppImage install/usr/share/applications/*.desktop -appimage

#cd install/usr/bin
#linuxdeployqt quokkagit
#cd -

#cpack -G TGZ
