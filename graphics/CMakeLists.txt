cmake_minimum_required(VERSION 3.15...4.4)

set( INC_FILES 
    inc/graphics/Window.hpp
)

set( SRC_FILES 
    src/Window.cpp
)

set ( IMGUI_INC_FILES
    ../external/imgui/imconfig.h
    ../external/imgui/imgui.h
    ../external/imgui/imgui_internal.h
    ../external/imgui/imstb_rectpack.h
    ../external/imgui/imstb_textedit.h
    ../external/imgui/imstb_truetype.h
)

set( IMGUI_SRC_FILES
    ../external/imgui/imgui.cpp
    ../external/imgui/imgui_demo.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
)

set( IMGUI_BACKEND_FILES
    ../external/imgui/backends/imgui_impl_sdl3.cpp
    ../external/imgui/backends/imgui_impl_sdl3.h
    ../external/imgui/backends/imgui_impl_sdlrenderer3.cpp
    ../external/imgui/backends/imgui_impl_sdlrenderer3.h
)

set ( IMGUI_FILES
    ${IMGUI_INC_FILES}
    ${IMGUI_SRC_FILES}
    ${IMGUI_BACKEND_FILES}
)

set ( ALL_FILES  
    ${INC_FILES}
    ${SRC_FILES}
    ${IMGUI_FILES}
    ../.clang-format
)

source_group(imgui FILES ${IMGUI_INC_FILES} ${IMGUI_SRC_FILES})
source_group(imgui/backends FILES ${IMGUI_BACKEND_FILES})

add_library( graphics STATIC ${ALL_FILES} )
add_library( sdl3_rast::graphics ALIAS graphics )

target_compile_features( graphics PUBLIC cxx_std_20 )

target_include_directories( graphics 
    PUBLIC inc
    PUBLIC ../external/imgui
    PRIVATE ../external/imgui/backends
)

target_link_libraries( graphics 
    PUBLIC 
        SDL3::SDL3
)

# Warning Level 4 and treat warnings as errors
target_compile_options( graphics PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic -Werror>
)