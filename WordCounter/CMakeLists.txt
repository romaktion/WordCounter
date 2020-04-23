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
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libasync/libasync.dll ${CMAKE_CURRENT_BINARY_DIR}/libasync.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libasync/libasync.pdb ${CMAKE_CURRENT_BINARY_DIR}/libasync.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libasync/libasync.ilk ${CMAKE_CURRENT_BINARY_DIR}/libasync.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libtext.dll ${CMAKE_CURRENT_BINARY_DIR}/libtext.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libtext.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtext.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libtext.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtext.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libiconv.dll ${CMAKE_CURRENT_BINARY_DIR}/libiconv.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libiconv.pdb ${CMAKE_CURRENT_BINARY_DIR}/libiconv.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtext/libiconv.ilk ${CMAKE_CURRENT_BINARY_DIR}/libiconv.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtiming/libtiming.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiming.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtiming/libtiming.pdb ${CMAKE_CURRENT_BINARY_DIR}/libtiming.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libtiming/libtiming.ilk ${CMAKE_CURRENT_BINARY_DIR}/libtiming.ilk)

    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libwordcounter/libwordcounter.dll ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libwordcounter/libwordcounter.pdb ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libwordcounter/libwordcounter.ilk ${CMAKE_CURRENT_BINARY_DIR}/libwordcounter.ilk)
endif ()

#default input text file for test
add_custom_command(TARGET WordCounter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}
)
