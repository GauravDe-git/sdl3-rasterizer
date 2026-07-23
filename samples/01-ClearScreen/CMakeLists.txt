cmake_minimum_required(VERSION 3.15...4.4)

add_executable(01-ClearScreen main.cpp ../../.clang-format)

target_link_libraries(01-ClearScreen 
        PRIVATE 
        sdl3_rast::graphics
)