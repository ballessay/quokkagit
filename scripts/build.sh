#!/bin/bash

BUILD_DIR=build
INSTALL_DIR=install

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DBUNDLE_LIBGIT2=ON ..

make -j

DESTDIR=$INSTALL_DIR make install

rm -rf $INSTALL_DIR/usr/include

