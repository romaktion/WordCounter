# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.10.2)

project ("WordCounter")
set (CMAKE_CXX_STANDARD 17)

# Include sub-projects.

#libs
add_subdirectory ("libasync")
add_subdirectory ("libtext")
add_subdirectory ("libtiming")
add_subdirectory ("libwordcounter")

#exec
if (NOT ANDROID)
	add_subdirectory ("WordCounter")
endif()
