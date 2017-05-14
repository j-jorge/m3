#!/bin/sh

# This is the directory containing this script
SCRIPT_DIR=$(cd $(dirname $0); pwd)/build-scripts

. $SCRIPT_DIR/config.sh

cd "$SOURCE_ROOT"

run_all_scripts()
{
    ROOT=$1
    shift
    
    for f in $ROOT/*.sh
    do
        if [ -x $f ]
        then
            echo "\033[01;34mExecuting $f\033[00m"
            $f $@
        fi
    done
}

run_all_scripts $SCRIPT_DIR $@ 2>&1 | tee build.log
