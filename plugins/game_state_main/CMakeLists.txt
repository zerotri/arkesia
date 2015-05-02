cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(Arkesia_plugins_game_state_main)

set(ARKESIA    "${CMAKE_SOURCE_DIR}")
set(sources
	game_state_main.cpp
)
include_directories(
	${ARKESIA}/src
)

add_library(game_state_main ${sources})
if (APPLE)
    set_property(TARGET game_state_main PROPERTY PREFIX "")
    set_property(TARGET game_state_main PROPERTY OUTPUT_NAME "game_state_main.dylib")
    set_property(TARGET game_state_main PROPERTY SUFFIX "")
endif()