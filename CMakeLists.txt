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

option(USE_STB_IMAGE "Use stb_image even if SDL2_image is available" OFF)
if(NOT USE_STB_IMAGE)
    find_package(SDL2_image QUIET)
endif()

include(FetchContent)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG        v2.0.0rc13
)
FetchContent_MakeAvailable(tinyobjloader)

if(SDL2_image_FOUND)
    message(STATUS "Using SDL2_image")
else()
    message(STATUS "Using stb_image")
    set(STB_COMMIT 31c1ad37456438565541f4919958214b6e762fb4)
    set(STB_DIR "${CMAKE_BINARY_DIR}/_deps/stb")
    set(STB_IMAGE_H "${STB_DIR}/stb_image.h")
    file(MAKE_DIRECTORY "${STB_DIR}")
    file(
        DOWNLOAD
        "https://raw.githubusercontent.com/nothings/stb/${STB_COMMIT}/stb_image.h"
        "${STB_IMAGE_H}"
        SHOW_PROGRESS
    )
endif()

add_library(image_loader INTERFACE)

if(SDL2_image_FOUND)
    target_link_libraries(image_loader
        INTERFACE
            SDL2_image::SDL2_image
    )
    target_compile_definitions(image_loader
        INTERFACE
            USE_SDL2_IMAGE=1
    )
else()
    target_include_directories(image_loader
        INTERFACE
            "${STB_DIR}"
    )
endif()

add_subdirectory(ZFX)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_subdirectory(Demo)
endif()
