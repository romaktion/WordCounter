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
        CONFIGURE_COMMAND       ""
        BUILD_COMMAND           ""
        INSTALL_COMMAND         ""
        UPDATE_COMMAND          ""
    )

    #set dir
    ExternalProject_Get_Property(libiconv_ext SOURCE_DIR)
    set(LIBICONV_DIR ${SOURCE_DIR})
    include_directories(${LIBICONV_DIR}/include)

    #create library
    set(SOURCES 
        ${LIBICONV_DIR}/libiconv/iconv.c
        ${LIBICONV_DIR}/libiconv/localcharset.c
        ${LIBICONV_DIR}/libiconv/relocatable.c
        )
    set_source_files_properties(${SOURCES} PROPERTIES GENERATED TRUE)
    add_library(libiconv STATIC ${SOURCES})
    add_dependencies(libiconv libiconv_ext)
    target_include_directories(libiconv INTERFACE ${LIBICONV_DIR}/include)
    
    install(TARGETS libiconv
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

    #link libiconv to WordCounter
    target_include_directories(WordCounter PRIVATE ${LIBICONV_DIR}/include)
    target_link_libraries(WordCounter libiconv)

elseif(UNIX)
    include(ExternalProject)

    set(PREFIX /usr/local)

	ExternalProject_Add(libiconv_linux
	URL				   https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz
	CONFIGURE_COMMAND  <SOURCE_DIR>/configure --prefix=${PREFIX}
	BUILD_COMMAND	   make
	INSTALL_COMMAND	   sudo make install
	)

    target_link_libraries(WordCounter PRIVATE ${PREFIX}/lib/libiconv.so ${CMAKE_THREAD_LIBS_INIT})
endif ()

install(TARGETS WordCounter RUNTIME DESTINATION bin)

add_custom_command(TARGET WordCounter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}
)
