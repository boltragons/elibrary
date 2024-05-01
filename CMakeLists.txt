# Copyright (c) 2024 eLib
# All rights reserved

set(CMAKE_C_COMPILER gcc)

cmake_minimum_required(VERSION ${CMAKE_VERSION})
project("eLibrary" C)

set(elib_dir ${PROJECT_SOURCE_DIR})

add_subdirectory(${elib_dir}/module)
add_subdirectory(${elib_dir}/view)
add_subdirectory(${elib_dir}/utils)
add_subdirectory(${elib_dir}/app)
