file(GLOB_RECURSE DEMO_SOURCES
    *.cpp
)

add_executable(zfx_demo ${DEMO_SOURCES})

target_compile_options(zfx_demo PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -Wall>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

target_include_directories(zfx_demo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(zfx_demo
    PRIVATE
        ZFX
)

