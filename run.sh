#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status.

if [ ! -d build ]; then
  mkdir build
fi
cd build

cmake ..
make

if [ -f typing_test ]; then
  echo "Moving typing_test to /usr/local/bin..."
  sudo mv typi /usr/local/bin/
else
  echo "Error: typing_test binary not found!"
  exit 1
fi

typing_test
