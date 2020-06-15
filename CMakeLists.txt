cmake_minimum_required(VERSION 3.16)
project(radiostation_dll)

set(CMAKE_CXX_STANDARD 14)

add_library(radiostation_dll SHARED library.cpp library.h src/tools/Socket.cpp src/tools/Socket.h src/controllers/NetworkController.cpp src/controllers/NetworkController.h src/model/Client.cpp src/model/Client.h src/model/message.h src/model/Server.cpp src/model/Server.h)