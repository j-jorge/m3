#!/bin/bash

set -e

. $(dirname $0)/config.sh

if [ $(build_mode) = "release" ]
then
    LOCAL_CFLAGS="-O3 -DNDEBUG"
else
    LOCAL_CFLAGS="-O0 -g -D_DEBUG"
fi

LOCAL_CXXFLAGS="$CXXFLAGS $LOCAL_CFLAGS --std=c++11"

TARGET_DIR="$M3_APK_ROOT/java/libs/$ANDROID_ABI/"

[ -d "$TARGET_DIR" ] || mkdir --parents "$TARGET_DIR"

M3_LIB_NAME=libm3.so
TARGET_SO="$TARGET_DIR/$M3_LIB_NAME"
SOURCES="$SDL_SOURCE_DIR/src/main/android/SDL_android_main.c"

compile_object()
{
    if [ $2 -nt $1 ]
    then
        printf "\033[01;33mUp to date:\033[00m %s\n" "$2"
        return
    fi

    mkdir --parents $(dirname "$2")
    
    if [[ $1 = *.c ]]
    then
        COMPILER="$ANDROID_CC $CFLAGS $LOCAL_CFLAGS"
    else
        COMPILER="$ANDROID_CXX $CXXFLAGS $LOCAL_CXXFLAGS"
    fi

    printf "\033[01;34mCompiling\033[00m %s\n" "$2"

    $COMPILER \
             -fPIC \
             -I"$SDL_SOURCE_DIR/src/main/android/" \
             -I$INSTALL_PREFIX/include \
             -I$INSTALL_PREFIX/include/SDL2 \
             -c "$1" \
             -o "$2"
}

compile()
{
    for f in $SOURCES
    do
        OBJECT_FILE=$CACHE/apk-lib/$(echo $f | sed 's|c\(pp\)\?$|o|g')
        OBJECTS="$OBJECTS $OBJECT_FILE"
        compile_object $f $OBJECT_FILE
    done
        
    printf "\033[01;31mLinking\033[00m %s\n" "$TARGET_SO"

    $ANDROID_CXX $CXXFLAGS $LDFLAGS \
             -fPIC -shared \
             -Wl,-soname,$M3_LIB_NAME \
             -Wl,--export-dynamic -rdynamic \
             $OBJECTS \
             -o "$TARGET_SO" \
             -L$INSTALL_PREFIX/lib \
             -L$M3_INSTALL_PREFIX/lib \
             -Wl,--no-as-needed -Wl,--whole-archive \
             -lm3 \
             -lm3_items \
             -lbear_generic_items \
             -Wl,--as-needed -Wl,--no-whole-archive \
             -lm3_core \
             -lm3_boost \
             -lm3_math \
             -lm3_stl \
             -lbear_engine \
             -lbear_audio \
             -lbear_communication \
             -lbear_expr \
             -lbear_gui \
             -lbear_input \
             -lbear_net \
             -lbear_text_interface \
             -lbear_time \
             -lbear_universe \
             -lbear_visual \
             -lbear_debug \
             -Wl,-Bstatic \
             -lclaw_tween \
             -lclaw_graphic \
             -lclaw_net \
             -lclaw_application \
             -lclaw_configuration_file \
             -lclaw_dynamic_library \
             -lclaw_logger \
             -lfreetype \
             -lpng \
             -ljpeg \
             -lboost_thread \
             -lboost_regex \
             -lboost_signals \
             -lboost_filesystem \
             -lboost_system \
             -lintl \
             -lSDL2_mixer \
             -lSDL2 \
             -lbreakpad_client \
             -lsupc++ \
             -lstdioext \
             -Wl,-Bdynamic \
             -lgnustl_shared \
             -lz \
             -lGLESv2 \
             -lGLESv1_CM \
             -llog \
             -ldl \
             -landroid
}

mkdir -p "$CACHE"/apk-lib/
compile

cp $ANDROID_TOOLCHAIN_ROOT/arm-linux-androideabi/lib/libgnustl_shared.so \
    "$TARGET_DIR"

/home/julien/local/android-toolchain/arm-linux-androideabi/bin/nm \
    --demangle \
    "$TARGET_DIR"/libm3.so \
    > $SOURCE_ROOT/symbols.txt
    
$ANDROID_TOOLCHAIN_ROOT/bin/arm-linux-androideabi-strip --strip-all \
    "$TARGET_DIR"/*.so \

SYMBOLS=$ARTEFACTS/libs.zip

rm -f "$SYMBOLS"
zip --junk-paths -9 "$SYMBOLS" "$TARGET_SO"

set_shell_variable M3_SYMBOLS $SYMBOLS
