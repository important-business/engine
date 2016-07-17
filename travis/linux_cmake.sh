#!/usr/bin/env sh

# Note: You require the following line in .travis.yml in addition to running
# this script:
#  - if [ "$TRAVIS_OS_NAME" = "linux" ]; then export PATH=$HOME/cmake-3.3.1-Linux-x86_64/bin:$PATH ; fi

echo "########################################################################"
echo "# Travis CMake upgrade script"
echo "########################################################################"

wget https://cmake.org/files/v3.3/cmake-3.3.1-Linux-x86_64.tar.gz -O cmake.tar.gz --no-check-certificate
tar -xzf cmake.tar.gz -C $HOME

export PATH=$HOME/cmake-3.3.1-Linux-x86_64/bin:$PATH
