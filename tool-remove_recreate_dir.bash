#!/bin/bash

set -e

current_dir=`readlink -f .`
expect_build=`echo "${current_dir}" |  awk -F '/' '{print $NF}' | cut -d '-' -f1`
if [[ "$expect_build" != "build" ]] ; then
    echo "Not a build directory. Doing nothing."
    exit
fi

cd ..
rm "$current_dir" -rf
mkdir "$current_dir"
cd "$current_dir"