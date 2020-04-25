cmake_minimum_required (VERSION 3.10)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/await.cpp
        )

add_library(libasync SHARED ${SOURCES})
