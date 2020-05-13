#cmake_minimum_required (VERSION 3.10)

get_target_property(LIBTEXT_SOURCE libtext SOURCE_DIR)
get_target_property(LIBTIMING_SOURCE libtiming SOURCE_DIR)

include_directories(${LIBTEXT_SOURCE} ${LIBTIMING_SOURCE})

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/wordcounter.cpp
        )

add_library(libwordcounter SHARED ${SOURCES})

target_include_directories(libwordcounter PRIVATE ${LIBTEXT_SOURCE} ${LIBTIMING_SOURCE})
target_link_libraries(libwordcounter PUBLIC libtext libtiming)
