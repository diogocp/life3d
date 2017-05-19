cmake_minimum_required(VERSION 2.8.12)
project(life3d LANGUAGES C)

add_definitions(-D_POSIX_SOURCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall -msse4.2")

set(SHARED_SOURCE_FILES main.c cell.c hashtable.c io.c)

add_executable(life3d life3d.c ${SHARED_SOURCE_FILES})

find_package(OpenMP)
if (OPENMP_FOUND)
    add_executable(life3d-omp life3d-omp.c ${SHARED_SOURCE_FILES})
    target_compile_options(life3d-omp PRIVATE ${OpenMP_C_FLAGS})
    target_link_libraries(life3d-omp PRIVATE ${OpenMP_C_FLAGS})
else()
    message(WARNING "OpenMP not found; skipping")
endif()

find_package(MPI)
if (MPI_C_FOUND)
    add_executable(life3d-mpi life3d-mpi.c ${SHARED_SOURCE_FILES})
    target_compile_definitions(life3d-mpi PRIVATE LIFE3D_MPI)
    target_compile_options(life3d-mpi PRIVATE ${MPI_C_COMPILE_FLAGS})
    target_include_directories(life3d-mpi PRIVATE ${MPI_C_INCLUDE_PATH})
    target_link_libraries(life3d-mpi PRIVATE ${MPI_C_LIBRARIES})
else()
    message(WARNING "MPI not found; skipping")
endif()


enable_testing()

# Serial version tests
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
add_test(NAME s500e300k.2000
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s500e300k.in 2000 | cmp s500e300k.2000.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
add_test(NAME s500e5M.10
        COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d s500e5M.in 10 | cmp s500e5M.10.out -"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)

# OpenMP version tests
if (OPENMP_FOUND)
    add_test(NAME s5e50.10-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s5e50.in 10 | cmp s5e50.10.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s50e5k.300-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s50e5k.in 300 | cmp s50e5k.300.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s20e400.500-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s20e400.in 500 | cmp s20e400.500.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s150e10k.1000-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s150e10k.in 1000 | cmp s150e10k.1000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s200e50k.1000-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s200e50k.in 1000 | cmp s200e50k.1000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s500e300k.2000-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s500e300k.in 2000 | cmp s500e300k.2000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s500e5M.10-omp
            COMMAND sh -c "${PROJECT_BINARY_DIR}/life3d-omp s500e5M.in 10 | cmp s500e5M.10.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
endif()

# MPI version tests
if (MPI_C_FOUND)
    add_test(NAME s5e50.10-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s5e50.in 10 | cmp s5e50.10.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s50e5k.300-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s50e5k.in 300 | cmp s50e5k.300.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s20e400.500-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s20e400.in 500 | cmp s20e400.500.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s150e10k.1000-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s150e10k.in 1000 | cmp s150e10k.1000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s200e50k.1000-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s200e50k.in 1000 | cmp s200e50k.1000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s500e300k.2000-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s500e300k.in 2000 | cmp s500e300k.2000.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
    add_test(NAME s500e5M.10-mpi
            COMMAND sh -c "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} ${PROJECT_BINARY_DIR}/life3d-mpi ${MPIEXEC_POSTFLAGS} s500e5M.in 10 | cmp s500e5M.10.out -"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/instances/)
endif()
