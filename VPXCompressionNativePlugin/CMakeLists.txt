cmake_minimum_required(VERSION 3.17)
project(VPXCompressionNativePlugin LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

################################################################################
# Build Options

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# In debug mode, add -DDEBUG
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # Warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

    # Remove Asio warning
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")

    # Static library: -fPIC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -march=native -fstack-protector")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
endif()

################################################################################
# Libraries

find_path(VPX_INCLUDE_DIR vpx)
find_library(VPX_LIBRARY vpxmd)
message(STATUS "VPX_INCLUDE_DIR: ${VPX_INCLUDE_DIR}")

################################################################################
# Source

set(DLL_SOURCE_FILES
    src/IUnityInterface.h
    src/vp8_encoder.h
    src/vp8_encoder.cpp
    src/vp8_decoder.h
    src/vp8_decoder.cpp
    src/plugin.cpp
)

################################################################################
# Targets

add_library(VPXCompressionNativePlugin SHARED ${DLL_SOURCE_FILES})
target_include_directories(VPXCompressionNativePlugin PRIVATE ${VPX_INCLUDE_DIR})
target_link_libraries(VPXCompressionNativePlugin PRIVATE ${VPX_LIBRARY})

################################################################################