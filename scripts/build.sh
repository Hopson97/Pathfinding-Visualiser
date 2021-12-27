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
    if [ -d "$pathfinding_visualiser " ]
    then
        rm -rf -d pathfinding_visualiser 
    fi

    mkdir -p pathfinding_visualiser 
fi

# Creates the folder for the buildaries
mkdir -p pathfinding_visualiser
mkdir -p pathfinding_visualiser/Data
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
    cp build/release/bin/pathfinding_visualiser  pathfinding_visualiser/pathfinding_visualiser 
else
    target_debug
fi

cp -R data pathfinding_visualiser/
