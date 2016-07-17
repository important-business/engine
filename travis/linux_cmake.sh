#!/usr/bin/env sh
# Note: This file should be sourced

echo "########################################################################"
echo "# Travis CMake upgrade script"
echo "########################################################################"

wget https://cmake.org/files/v3.3/cmake-3.3.1-Linux-x86_64.tar.gz -O cmake.tar.gz --no-check-certificate
tar -xzf cmake.tar.gz -C $HOME

export PATH=$HOME/cmake-3.3.1-Linux-x86_64/bin:$PATH
