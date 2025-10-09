#!/bin/bash
set -e

# This script builds and runs the typi project.

# Create a build directory if it doesn't exist
mkdir -p build
cd build

# Configure the project with CMake
cmake ..

# Build the project
make

# Run the executable
./typi
