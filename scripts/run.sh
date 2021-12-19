#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/pathfinding_visualisor 
else
    ./build/debug/bin/pathfinding_visualisor 
fi