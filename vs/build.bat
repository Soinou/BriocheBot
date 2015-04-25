@echo off
cd ..
mkdir build 2> NUL
cd build
cmake ../ -GNinja -DCMAKE_BUILD_TYPE:String=Release
ninja