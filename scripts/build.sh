#!/bin/bash

BUILD_DIR=builds

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUNDLE_LIBGIT2=ON ..

make -j

DESTDIR=install make install

#cd install/usr/bin
#linuxdeployqt quokkagit
#cd -

cpack -G TGZ
