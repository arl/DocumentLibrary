#####################################################
#													#
#		DocLibrary main cmake configuration file	#
#													#
#####################################################

project (DOCLIBRARY CXX)
set (DOCLIBRARY_VERSION_MAJOR 0)
set (DOCLIBRARY_VERSION_MINOR 1)
set (DOCLIBRARY_VERSION_BUILD 16)
set (CMAKE_BUILD_TYPE Debug)
cmake_minimum_required (VERSION 2.6)


# check for correct version of Boost
INCLUDE(FindBoost)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE( Boost 1.42 COMPONENTS system filesystem program_options )

enable_testing()

# sets binary and source directories
#set (DOCLIBRARY_SOURCE_DIR ".")
set (DOCLIBRARY_SOURCE_DIR "/home/orl/dev/DocumentLibrary")
set (DOCLIBRARY_BINARY_DIR "/home/orl/dev/doclib_install")

message ("currently generating : ${CMAKE_GENERATOR}")

# define program to read a file (platform-wise)
#if(WIN32)
	#set(CAT_PROG type)
#elseif(UNIX)
	#set(CAT_PROG cat)
#endif(WIN32)

# add cmake subprojects
add_subdirectory (src/core)
add_subdirectory (src/libmgr)
add_subdirectory (src/daemon)
add_subdirectory (test)

# add pre build step : increment build number
#add_custom_command (OUTPUT "${PROJECT_SOURCE_DIR}/")

# add post build step : archive source files
#add_custom_command (TARGET doclibd
					#POST_BUILD
					#COMMAND backup_dir -i '*.cpp' -i '*.hpp' -i '*.c' -i '*.h' doclibd.src "${DOCLIBRARY_SOURCE_DIR}../backup"
					#WORKING_DIR "${DOCLIBRARY_SOURCE_DIR}"
					#VERBATIM "coucou"
					#)
# create timestamp
#execute_process (COMMAND date "+%y%m%d.%H%M" OUTPUT_VARIABLE _date)
#message(STATUS "date is ${_date}")

# some tests to see if everything went ok
add_test (LibManagerRuns "${DOCLIBRARY_BINARY_DIR}/test/bin/libmgr_test")
