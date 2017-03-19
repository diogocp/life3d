cmake_minimum_required(VERSION 3.5)
project(life3d)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -msse4.2)

set(SOURCE_FILES life3d.cpp Grid.cpp Grid.h HashTable.cpp HashTable.h)
add_executable(life3d ${SOURCE_FILES})
