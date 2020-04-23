cmake_minimum_required (VERSION 3.15)

include_directories(${CMAKE_SOURCE_DIR}/libtext ${CMAKE_SOURCE_DIR}/libtiming)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/wordcounter.cpp
        )

add_library(libwordcounter SHARED ${SOURCES})

target_include_directories(libwordcounter PRIVATE ${CMAKE_SOURCE_DIR}/libtext)
target_link_libraries(libwordcounter PUBLIC libtext)

target_include_directories(libwordcounter PRIVATE ${CMAKE_SOURCE_DIR}/libtiming)
target_link_libraries(libwordcounter PUBLIC libtiming)
