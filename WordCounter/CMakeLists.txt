# CMakeList.txt : CMake project for WordCounter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.

add_executable(WordCounter main.cpp pch.cpp)

get_target_property(LIBASYNC_SOURCE libasync SOURCE_DIR)
get_target_property(LIBTIMING_SOURCE libtiming SOURCE_DIR)
get_target_property(LIBWORDCOUNTER_SOURCE libwordcounter SOURCE_DIR)

include_directories(${LIBASYNC_SOURCE} ${LIBWORDCOUNTER_SOURCE} ${LIBTIMING_SOURCE})

target_include_directories(WordCounter PRIVATE ${LIBWORDCOUNTER_SOURCE})
target_link_libraries(WordCounter PRIVATE libwordcounter libasync libtiming)

# TODO: Add tests and install targets if needed.
if(UNIX)
    find_package(Threads)
    target_link_libraries(WordCounter PRIVATE ${CMAKE_THREAD_LIBS_INIT})
elseif(WIN32)
    #copy libs dlls

    get_target_property(LIBASYNC_BINARY libasync BINARY_DIR)
    get_target_property(LIBTEXT_BINARY libtext BINARY_DIR)
    get_target_property(LIBTIMING_BINARY libtiming BINARY_DIR)
    get_target_property(LIBWORDCOUNTER_BINARY libwordcounter BINARY_DIR)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_BINARY}/libasync.dll ${CMAKE_CURRENT_BINARY_DIR}/libasync.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_BINARY}/libasync.pdb ${CMAKE_CURRENT_BINARY_DIR}/libasync.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_BINARY}/libasync.ilk ${CMAKE_CURRENT_BINARY_DIR}/libasync.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libtext.dll ${CMAKE_CURRENT_BINARY_DIR}/libtext.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libtext.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtext.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libtext.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtext.ilk)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libiconv.dll ${CMAKE_CURRENT_BINARY_DIR}/libiconv.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libiconv.pdb ${CMAKE_CURRENT_BINARY_DIR}/libiconv.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_BINARY}/libiconv.ilk ${CMAKE_CURRENT_BINARY_DIR}/libiconv.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_BINARY}/libtiming.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiming.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_BINARY}/libtiming.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtiming.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_BINARY}/libtiming.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtiming.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_BINARY}/libwordcounter.dll ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_BINARY}/libwordcounter.pdb ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_BINARY}/libwordcounter.ilk ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.ilk)
endif ()

#default input text file for test
add_custom_command(TARGET WordCounter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}
)
