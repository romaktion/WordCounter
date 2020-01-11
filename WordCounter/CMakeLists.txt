# CMakeList.txt : CMake project for WordCounter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
find_package( Threads )
file(GLOB Sources "*.cpp")
set(HEADER_FILES await.h constants.h )
add_executable (WordCounter ${Sources} ${HEADER_FILES})

# TODO: Add tests and install targets if needed.
add_definitions(-DBUILDING_LIBICONV -DBUILDING_LIBCHARSET)
IF (WIN32)
    TARGET_INCLUDE_DIRECTORIES (WordCounter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/iconv/include)
ENDIF ()

IF (UNIX)
    TARGET_LINK_LIBRARIES (WordCounter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/iconv/lib/linux/libiconv.so ${CMAKE_THREAD_LIBS_INIT})
ELSEIF (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        TARGET_LINK_LIBRARIES (WordCounter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/iconv/lib/win32/libiconv_x64.lib)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        TARGET_LINK_LIBRARIES (WordCounter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/iconv/lib/win32/libiconv_x86.lib)
    endif()
ENDIF ()
