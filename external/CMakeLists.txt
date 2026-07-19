cmake_minimum_required(VERSION 3.15...4.4)

set( SDL_TEST_LIBRARY OFF CACHE BOOL "building the SDL3_test library" )
set (SDL_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstall of SDL3" )
set( AUDIO_BUILD_EXAMPLES OFF CACHE BOOL " build samples for the 3dgep audio examples" )

add_subdirectory(Audio)
add_subdirectory(glm)
add_subdirectory(SDL)
add_subdirectory(SDL_ttf)
add_subdirectory(input)

set( SDL_TARGETS 
    SDL_uclibc SDL3-static SDL3-shared SDL3-ttf SDL3_ttf-static
    freetype harfbuzz plutosvg plutovg 
    )

set( GLM_TARGETS glm glm_shared
    )  

macro(set_targets_folder TARGETS FOLDER)
    foreach(target ${TARGETS})
        if (TARGET ${target})
            set_target_properties(${target} PROPERTIES FOLDER ${FOLDER})
        endif()
    endforeach(target ${TARGETS})
endmacro()

set_targets_folder("${GLM_TARGETS}" "external/glm")
set_targets_folder("${SDL_TARGETS}" "external/SDL")
set_targets_folder( "Audio;input_SDL3" "external" )