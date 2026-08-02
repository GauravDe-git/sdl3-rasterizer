cmake_minimum_required(VERSION 3.15...4.4)

set (TARGET_NAME 00-Common)

add_library(${TARGET_NAME} STATIC Timer.hpp Timer.cpp)

# Expose this folder as an include dir so samples can #include <Timer.hpp>
target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        sdl3_rast::graphics
)