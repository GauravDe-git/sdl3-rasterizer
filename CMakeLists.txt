cmake_minimum_required(VERSION 3.15...4.4)

option( BUILD_SHARED_LIBS "Build using shared libraries" OFF )
option( SDL3_RAST_SAMPLES "Build samples for the sdl3 rasterizer library" ON )

set( SDL3_RAST_VERSION_MAJOR 0 )
set( SDL3_RAST_VERSION_MINOR 0 )
set( SDL3_RAST_VERSION_PATCH 1 )
set( SDL3_RAST_VERSION "${SDL3_RAST_VERSION_MAJOR}.${SDL3_RAST_VERSION_MINOR}.${SDL3_RAST_VERSION_PATCH}" )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( sdl3_rast VERSION ${SDL3_RAST_VERSION} LANGUAGES C CXX )

if(MSVC)
    add_compile_options(/MP)
endif(MSVC)

add_subdirectory( external EXCLUDE_FROM_ALL)
# add_subdirectory( math )
# add_subdirectory( graphics )

if(SDL3_RAST_SAMPLES)
#    add_subdirectory( samples )
endif(SDL3_RAST_SAMPLES)