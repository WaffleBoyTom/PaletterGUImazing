# CMake

message(STATUS "CMake project-level configuration")

cmake_minimum_required(VERSION 3.16)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CXX
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project
project(PaletterGUImazing VERSION 0.1 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

if(APPLE)
    find_package(OpenGL REQUIRED)
    set(CMAKE_MACOSX_RPATH ON)
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET 13.0)

# Compute backend
message(STATUS "Selecting compute backend")

set(USE_CUDA FALSE)
set(USE_METAL FALSE)

find_package(CUDAToolkit)

if (CUDAToolkit_FOUND)
    message(STATUS "CUDA toolkit found at: ${CUDAToolkit_LIBRARY_DIR}")

    enable_language(CUDA)

    set(USE_CUDA TRUE)

    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)

elseif(APPLE)
    find_library(METAL_LIB Metal REQUIRED)
    find_library(FOUNDATION_LIB Foundation REQUIRED)
    find_library(QUARTZCORE_LIB QuartzCore REQUIRED)

    if (METAL_LIB)
        message(STATUS "Metal library found at: ${METAL_LIB}")
        message(STATUS "Foundation library found at: ${FOUNDATION_LIB}")
        message(STATUS "QuartzCore library found at: ${QUARTZCORE_LIB}")

        set(USE_METAL TRUE)
    endif()

else()
    message(STATUS "No compute platform discovered")

endif()

# Qt
message(STATUS "Searching for Qt package")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

add_subdirectory(src)
