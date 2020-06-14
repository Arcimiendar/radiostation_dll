cmake_minimum_required(VERSION 3.16)
project(radiostation_dll)

set(CMAKE_CXX_STANDARD 14)

add_library(radiostation_dll SHARED library.cpp library.h)