#!/usr/bin/env sh

echo "########################################################################"
echo "# Travix Linux Install Script"
echo "########################################################################"

apt-get install -qq libsdl2-dev libsdl2-image-dev liblog4cplus-dev gcc-4.8 g++-4.8
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 90
wget http://www.cmake.org/files/v3.3/cmake-3.3.1-Linux-x86_64.tar.gz -O cmake.tar.gz
tar -xzf cmake.tar.gz -C $HOME
