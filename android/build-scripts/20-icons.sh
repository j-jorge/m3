#!/bin/sh

set -e

. $(dirname $0)/config.sh

convert_icon()
{
    SOURCE="$1"

    find "$M3_APK_ROOT/java/res" -name "$2" \
        | while read TARGET_ICON
    do
        SIZE=$(identify -format "%wx%h" "$TARGET_ICON")
        convert "$SOURCE" -resize $SIZE "$TARGET_ICON"
    done
}

convert_icon "$SOURCE_ROOT/android/icons/launcher.png" "ic_launcher.png"
convert_icon "$SOURCE_ROOT/android/icons/notification.png" "ic_notification.png"
