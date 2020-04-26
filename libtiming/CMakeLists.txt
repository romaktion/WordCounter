#cmake_minimum_required (VERSION 3.10)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/timing.cpp
        )

add_library(libtiming SHARED ${SOURCES})
