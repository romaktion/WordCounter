# CMakeList.txt : CMake project for WordCounter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
find_package( Threads )
file(GLOB Sources "*.cpp" "await.h" "constants.h")
add_executable (WordCounter ${Sources})

# TODO: Add tests and install targets if needed.
add_definitions(-DBUILDING_LIBICONV -DBUILDING_LIBCHARSET)
if (WIN32)
    include(ExternalProject)

    #grab libiconv
    ExternalProject_Add(libiconv_ext
        GIT_REPOSITORY          https://github.com/romaktion/libiconv.git
        INSTALL_COMMAND         ${CMAKE_COMMAND} -E copy
                                <BINARY_DIR>/libiconv.lib
                                ${CMAKE_BINARY_DIR}/libiconv.lib
        UPDATE_COMMAND          ""
    )

    #set dirs
    ExternalProject_Get_Property(libiconv_ext SOURCE_DIR)
    ExternalProject_Get_Property(libiconv_ext BINARY_DIR)
    include_directories(${SOURCE_DIR}/include)

    #link libiconv to WordCounter
    target_include_directories(WordCounter PRIVATE ${SOURCE_DIR}/include)
    target_link_libraries(WordCounter PRIVATE libiconv)
    add_dependencies(WordCounter libiconv_ext)

    #copy libiconv binaries
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.dll ${CMAKE_CURRENT_BINARY_DIR}/libiconv.dll)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.pdb ${CMAKE_CURRENT_BINARY_DIR}/libiconv.pdb)
    add_custom_command(TARGET WordCounter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.ilk ${CMAKE_CURRENT_BINARY_DIR}/libiconv.ilk)

elseif(UNIX)
    include(ExternalProject)

    set(PREFIX /usr/local)

    #grab libiconv
	ExternalProject_Add(libiconv_ext
	URL				   https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz
	CONFIGURE_COMMAND  <SOURCE_DIR>/configure --prefix=${PREFIX}
	BUILD_COMMAND	   make
	INSTALL_COMMAND	   ""#sudo make install
	)

    #set dirs
    ExternalProject_Get_Property(libiconv_ext SOURCE_DIR)
    ExternalProject_Get_Property(libiconv_ext BINARY_DIR)
    include_directories(${SOURCE_DIR}/include)

    #copy libiconv binaries
    target_include_directories(WordCounter PRIVATE ${SOURCE_DIR}/include)
    target_link_libraries(WordCounter PRIVATE ${BINARY_DIR}/lib/.libs/libiconv.so ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(WordCounter libiconv_ext)
endif ()

#default input text file for test
add_custom_command(TARGET WordCounter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}
)
