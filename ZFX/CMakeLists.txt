file(GLOB_RECURSE ZFX_SOURCES
    src/*.cpp
)

add_library(ZFX STATIC ${ZFX_SOURCES})

target_compile_options(ZFX PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -Wall>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

target_include_directories(ZFX
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${tinyobjloader_SOURCE_DIR}
)

target_link_libraries(ZFX
    PUBLIC
        OpenGL
        GLEW
        Freetype::Freetype
        SDL2
        tinyobjloader
)
