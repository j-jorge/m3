#!/bin/sh

set -e

. $(dirname $0)/config.sh

M3_APK_ROOT=$CACHE/m3-apk

rm -fr "$M3_APK_ROOT"
cp -r "$SOURCE_ROOT/android" "$M3_APK_ROOT"

mkdir --parents $M3_APK_ROOT/java/src/org/libsdl/app/
sed 's|loadLibraries();||g' \
    $SDL_SOURCE_DIR/android-project/src/org/libsdl/app/SDLActivity.java \
    > $M3_APK_ROOT/java/src/org/libsdl/app/SDLActivity.java

BUILD_NUMBER_FILE="$CACHE/build-number"

if [ -f "$BUILD_NUMBER_FILE" ]
then
    BUILD_NUMBER=$(cat "$BUILD_NUMBER_FILE")
else
    BUILD_NUMBER=0
fi

sed 's|versionCode=".*"|versionCode="'$BUILD_NUMBER'"|' \
    -i $M3_APK_ROOT/java/AndroidManifest.xml
sed 's|versionName=".*"|versionName="'$M3_VERSION_FULL'"|' \
    -i $M3_APK_ROOT/java/AndroidManifest.xml
    
if [ $(build_mode $@) = debug ]
then
    cp -r "$SOURCE_ROOT/android/components/beta/"* \
       "$M3_APK_ROOT"
    
    sed 's|com\.stuffomatic\.m3|com.stuffomatic.beta.m3|' \
        -i $M3_APK_ROOT/java/AndroidManifest.xml \
        $M3_APK_ROOT/java/build.gradle

    sed 's|com\.stuffomatic\.m3\.R|com.stuffomatic.beta.m3.R|' \
        -i $M3_APK_ROOT/java/src/com/stuffomatic/apps/m3/*.java

    sed 's|Gem Ring|Beta Gem Ring|' \
        -i $M3_APK_ROOT/java/res/values/strings.xml
else
    cp -r "$SOURCE_ROOT/android/components/prod/"* \
       "$M3_APK_ROOT"

    echo $(( $BUILD_NUMBER + 1 )) > $BUILD_NUMBER_FILE
fi

set_shell_variable M3_APK_ROOT "$M3_APK_ROOT"
