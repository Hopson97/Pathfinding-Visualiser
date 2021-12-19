#!/bin/bash

target_release() {
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in build/release/"
    cd ../..
}

target_debug() {
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo "Built target in build/debug/"
    cd ../..
}

# Create folder for distribution
if [ "$1" = "release" ]
then
    if [ -d "$pathfinding_visualisor " ]
    then
        rm -rf -d pathfinding_visualisor 
    fi

    mkdir -p pathfinding_visualisor 
fi

# Creates the folder for the buildaries
mkdir -p pathfinding_visualisor
mkdir -p pathfinding_visualisor/Data
mkdir -p build
mkdir -p build/release
mkdir -p build/debug
cd build

if [ "$1" = "install" ]
then
    conan install .. -s compiler.libcxx=libstdc++11 --build=missing
fi

# Builds target
if [ "$1" = "release" ]
then
    target_release
    cp build/release/bin/pathfinding_visualisor  pathfinding_visualisor/pathfinding_visualisor 
else
    target_debug
fi

cp -R data pathfinding_visualisor/
