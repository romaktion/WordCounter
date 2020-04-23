# CMakeList.txt : CMake project for WordCounter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
find_package( Threads )
file(GLOB Sources "*.cpp")
add_executable(WordCounter ${Sources})

include_directories(${CMAKE_SOURCE_DIR}/libasync ${CMAKE_SOURCE_DIR}/libwordcounter ${CMAKE_SOURCE_DIR}/libtiming)

target_include_directories(WordCounter PRIVATE ${CMAKE_SOURCE_DIR}/libwordcounter)
target_link_libraries(WordCounter PRIVATE libwordcounter)

# TODO: Add tests and install targets if needed.
if(UNIX)
    target_link_libraries(WordCounter PRIVATE ${CMAKE_THREAD_LIBS_INIT})
elseif(WIN32)
    #copy libs dlls

    get_target_property(LIBASYNC_SOURCE libasync BINARY_DIR)
    get_target_property(LIBTEXT_SOURCE libtext BINARY_DIR)
    get_target_property(LIBTIMING_SOURCE libtiming BINARY_DIR)
    get_target_property(LIBWORDCOUNTER_SOURCE libwordcounter BINARY_DIR)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_SOURCE}/libasync.dll ${CMAKE_CURRENT_BINARY_DIR}/libasync.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_SOURCE}/libasync.pdb ${CMAKE_CURRENT_BINARY_DIR}/libasync.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBASYNC_SOURCE}/libasync.ilk ${CMAKE_CURRENT_BINARY_DIR}/libasync.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libtext.dll ${CMAKE_CURRENT_BINARY_DIR}/libtext.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libtext.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtext.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libtext.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtext.ilk)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libiconv.dll ${CMAKE_CURRENT_BINARY_DIR}/libiconv.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libiconv.pdb ${CMAKE_CURRENT_BINARY_DIR}/libiconv.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTEXT_SOURCE}/libiconv.ilk ${CMAKE_CURRENT_BINARY_DIR}/libiconv.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_SOURCE}/libtiming.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiming.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_SOURCE}/libtiming.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtiming.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBTIMING_SOURCE}/libtiming.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtiming.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_SOURCE}/libwordcounter.dll ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_SOURCE}/libwordcounter.pdb ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBWORDCOUNTER_SOURCE}/libwordcounter.ilk ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.ilk)
endif ()

#default input text file for test
add_custom_command(TARGET WordCounter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}
)
