# CMake

message(STATUS "CMake project-level configuration")

cmake_minimum_required(VERSION 3.16)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CXX
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -g \
    -D_THREAD_SAFE \
    -fms-extensions \
    -pedantic \
    -Wall \
    -Wno-attributes \
    -Werror \
    -Wpedantic"
)

# Project
project(PaletterGUImazing VERSION 0.1 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(APPLE)
    find_package(OpenGL REQUIRED)
    set(CMAKE_MACOSX_RPATH ON)
endif()

# TODO: bundle Qt lol. Then we can set the version; I had to go through some
# headaches of updating Qt on my machine since it was very outdated. Also now
# it expects to be built for MacOS 13.0 or greater; it would be nice if we
# didn't need to manually adjust the target version in the future
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
    find_library(METAL_LIB Metal)
    find_library(FOUNDATION_LIB Foundation)

    if (METAL_LIB)
        message(STATUS "Metal library found at: ${METAL_LIB}")
        message(STATUS "Foundation library found at: ${FOUNDATION_LIB}")

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

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(src)
