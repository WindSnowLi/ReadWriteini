cmake_minimum_required(VERSION 3.20)
project(rw_ini)

set(CMAKE_CXX_STANDARD 11)

add_executable(rw_ini_test test/test.cpp include/rw_ini_base.h)
