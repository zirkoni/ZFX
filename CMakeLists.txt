cmake_minimum_required(VERSION 3.16)
project(zfx_project CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

if(WIN32)
    message(STATUS "Compiling for Windows")

    if(NOT "$ENV{MSYSTEM}" STREQUAL "UCRT64")
        message(FATAL_ERROR "Not mingw-w64-ucrt environment")
    endif()
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

include(FetchContent)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG        v2.0.0rc13
)
FetchContent_MakeAvailable(tinyobjloader)

add_subdirectory(ZFX)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_subdirectory(Demo)
endif()

