@echo off
cd ..
mkdir build 2> NUL
cd build
cmake ../ -GNinja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE:String=Debug
ninja