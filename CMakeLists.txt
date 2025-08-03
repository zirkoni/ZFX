cmake_minimum_required(VERSION 3.16)
project(zfx_demo_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-g -O2)

if(WIN32)
    message(STATUS "Compiling for Windows")

    if(NOT "$ENV{MSYSTEM}" STREQUAL "UCRT64")
        message(FATAL_ERROR "Not mingw-w64-ucrt environment")
    endif()

    include_directories(/ucrt64/include/freetype2)

    set(EXTRA_LIBS
        OpenGL32
        glew32
        mingw32
        SDL2main
        SDL2
        freetype
    )
else()
    message(STATUS "Compiling for Linux")

    include_directories(/usr/include/freetype2)

    set(EXTRA_LIBS
        GL
        GLEW
        SDL2
        freetype
    )
endif()

set(EXTRA_LIBS ${EXTRA_LIBS} CACHE INTERNAL "Platform-specific libs")

include(FetchContent)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG        v2.0.0rc13
)

FetchContent_MakeAvailable(tinyobjloader)

add_subdirectory(ZFX)
add_subdirectory(Demo)

