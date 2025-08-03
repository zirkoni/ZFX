file(GLOB_RECURSE ZFX_SOURCES
    src/*.cpp
)

add_library(zfx STATIC ${ZFX_SOURCES})

target_include_directories(zfx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${tinyobjloader_SOURCE_DIR}
)

target_link_libraries(zfx
    PUBLIC
        ${EXTRA_LIBS}
)
