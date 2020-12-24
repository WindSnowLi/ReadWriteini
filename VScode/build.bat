@echo off
cd .\build
cmake -G "MinGW Makefiles" .. > TriffleInfo
mingw32-make