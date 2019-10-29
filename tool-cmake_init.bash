#!/bin/bash

set -e

if [ -f CMakeCache.txt ] ; then
    echo "CMake already initialized here. Doing nothing."
    exit
fi

current_path=`readlink -f .`
dirname=`echo "${current_path}" |  awk -F '/' '{print $NF}'`
expect_build=`echo "$dirname" | cut -d '-' -f1`
if [[ "$expect_build" != "build" ]] ; then
    echo "Not a build directory. Doing nothing."
    exit
fi

project=`echo "$dirname" | cut -d '-' -f2`

source_dir="../repository/vis-lite/${project}"
cmake "$source_dir" -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_INSTALL_PREFIX="../installed"
