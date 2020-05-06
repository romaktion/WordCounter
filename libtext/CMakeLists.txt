#cmake_minimum_required (VERSION 3.10)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/text.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/constants.h
        )

add_library(libtext SHARED ${SOURCES})

#grab libiconv
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

    #link libiconv to libtext
    target_include_directories(libtext PRIVATE ${SOURCE_DIR}/include)
    target_link_libraries(libtext PRIVATE libiconv)
    add_dependencies(libtext libiconv_ext)

    #copy libiconv binaries
    add_custom_command(TARGET libtext POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.dll ${CMAKE_CURRENT_BINARY_DIR}/libiconv.dll)
    add_custom_command(TARGET libtext POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.pdb ${CMAKE_CURRENT_BINARY_DIR}/libiconv.pdb)
    add_custom_command(TARGET libtext POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libiconv.ilk ${CMAKE_CURRENT_BINARY_DIR}/libiconv.ilk)

elseif(ANDROID)
	set(IMPORT_DIR ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI})
	
	include_directories(${IMPORT_DIR}/include)
	
	add_library(libiconv SHARED IMPORTED)
	set_target_properties(libiconv PROPERTIES IMPORTED_LOCATION ${IMPORT_DIR}/lib/libiconv.so)
	target_include_directories(libtext PRIVATE ${IMPORT_DIR}/include)

    target_link_libraries(libtext PRIVATE libiconv)
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
    target_include_directories(libtext PRIVATE ${SOURCE_DIR}/include)
    target_link_libraries(libtext PRIVATE ${BINARY_DIR}/lib/.libs/libiconv.so)
    add_dependencies(libtext libiconv_ext)
endif ()
