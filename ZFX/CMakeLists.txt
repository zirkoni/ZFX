file(GLOB_RECURSE ZFX_SOURCES
    src/*.cpp
)

add_library(ZFX STATIC ${ZFX_SOURCES})

target_include_directories(ZFX
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${tinyobjloader_SOURCE_DIR}
)

target_link_libraries(ZFX
    PUBLIC
        ${EXTRA_LIBS}
)
