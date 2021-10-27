#!/bin/bash
clear
echo " |-----| suppression des fichiers |-----| "
rm -rf cmake_install.cmake
rm -rf CMakeCache.txt
rm -rf Makefile
rm -rf CMakeFiles

echo " |-----| lancement de 'cmake .' |-----| "
cmake .

echo " |-----| lancement de 'make' |-----| "
make
