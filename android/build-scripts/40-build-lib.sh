#!/bin/bash

set -e

. $(dirname $0)/config.sh

BUILD_TYPE=$(build_mode $@)
BUILD_DIR="$CACHE/lib-build/$BUILD_TYPE"
mkdir --parents "$BUILD_DIR"

M3_INSTALL_PREFIX="$BUILD_DIR/install/"
M3_ASSETS_DIR_NAME="assets"

set_shell_variable M3_INSTALL_PREFIX $M3_INSTALL_PREFIX

configure()
{
    pushd "$BUILD_DIR"

    pwd

    echo "$CFLAGS"
    echo "$CXXFLAGS"
    echo "$ANDROID_CC"
    CC="$ANDROID_CC $CFLAGS" CXX="$ANDROID_CXX $CXXFLAGS" AR=$ANDROID_AR \
       cmake -B. -H$SOURCE_ROOT/cmake \
       -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
       -DCMAKE_PREFIX_PATH=$INSTALL_PREFIX \
       -DBoost_INCLUDE_DIR:PATH="$ANDROID_BOOST_INCLUDE_DIR" \
       -DBoost_USE_STATIC_RUNTIME=ON \
       -DPNG_PNG_INCLUDE_DIR:PATH="$ANDROID_PNG_INCLUDE_DIR" \
       -DPNG_LIBRARY:FILEPATH="$ANDROID_PNG_LIBRARY" \
       -DBUILD_PLATFORM="android" \
       -DCMAKE_INSTALL_PREFIX="$M3_INSTALL_PREFIX" \
       -DBEAR_ROOT_DIRECTORY="$BEAR_ROOT_DIR" \
       -DM3_INSTALL_DATA_DIR="$M3_ASSETS_DIR_NAME" \
       -DM3_INSTALL_EXECUTABLE_DIR="lib" \
       -DANDROID_BREAKPAD_INCLUDE_DIRECTORY="$ANDROID_BREAKPAD_INCLUDE_DIR" \
       -DANDROID_BREAKPAD_LIBRARIES="breakpad_client"
    popd
}

build()
{
    pushd "$BUILD_DIR"
    make -j$(nproc) install
    popd

    M3_INSTALLED_ASSETS="$M3_APK_ROOT/java/assets"
    
    if [ ! -L "$M3_INSTALLED_ASSETS" ]
    then
        ln -s "$M3_INSTALL_PREFIX/$M3_ASSETS_DIR_NAME" "$M3_INSTALLED_ASSETS"
    fi
}

configure
build
