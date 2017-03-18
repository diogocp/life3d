cmake_minimum_required(VERSION 3.5)
project(life3d)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES life3d.cpp Cell.cpp Cell.h Grid.cpp Grid.h HashTable.cpp HashTable.h MurmurHash3.cpp MurmurHash3.h)
add_executable(life3d ${SOURCE_FILES})
