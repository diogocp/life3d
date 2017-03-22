cmake_minimum_required(VERSION 2.8)
project(life3d)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -msse4.2")

set(SOURCE_FILES life3d.cpp io.cpp Grid.cpp Grid.h HashTable.cpp HashTable.h)
add_executable(life3d ${SOURCE_FILES})

enable_testing()
add_test(NAME s5e50.10
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s5e50.in 10 | cmp s5e50.10.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s50e5k.300
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s50e5k.in 300 | cmp s50e5k.300.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s20e400.500
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s20e400.in 500 | cmp s20e400.500.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s150e10k.1000
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s150e10k.in 1000 | cmp s150e10k.1000.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s200e50k.1000
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s200e50k.in 1000 | cmp s200e50k.1000.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s500e300k.2000.out
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s500e300k.in 2000 | cmp s500e300k.2000.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
