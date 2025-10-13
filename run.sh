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

# move the executable to /usr/local/bin 
sudo cp typi /usr/local/bin

# Run the executable
# Example: Start a typing test with default number of words
./typi

# Example: Start a typing test with 10 words
# ./typi -n 10

# Example: Start a typing test with 50 words
# ./typi -n 50

# Example: Show typing progress
# ./typi -sp

# Example: Show help message
# ./typi -h

