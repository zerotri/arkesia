#!/bin/bash
#

# variables
build_dir="tools/build"
executable="gen"
sources="$build_dir/build.cpp"

# build tool 'gen'
clang++ -o $build_dir/$executable $sources -std=c++1y
cp $build_dir/$executable ./$executable
