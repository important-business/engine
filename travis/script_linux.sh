#!/usr/bin/env sh
echo "########################################################################"
echo "# Travix Linux Script"
echo "########################################################################"
cd build
cmake ..
make

# Run unit tests with verbose output
make test ARGS='-V'

