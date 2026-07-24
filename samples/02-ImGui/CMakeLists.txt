cmake_minimum_required(VERSION 3.15...4.4)

set (TARGET_NAME 02-ImGui)

add_executable(${TARGET_NAME} main.cpp ../../.clang-format)

target_link_libraries(${TARGET_NAME} 
        PRIVATE 
        sdl3_rast::graphics
)