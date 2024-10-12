#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/pathfinding_visualiser 
else
    ./build/debug/bin/pathfinding_visualiser 
fi