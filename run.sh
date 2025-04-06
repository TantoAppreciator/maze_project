#!/bin/bash
if [ -f "build/maze_generator" ]; then
    ./build/maze_generator
else
    echo "Executable not found. Please run build.sh first"
    exit 1
fi