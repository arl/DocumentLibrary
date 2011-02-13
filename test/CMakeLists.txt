#############################################################
#	DocLibrary::test::libmgr_test cmake configuration file	#
#															#
# description : builds libmgr static library				#
#############################################################

# include source code directory
message ("${DOCLIBRARY_SOURCE_DIR}/src is included")
include_directories("${DOCLIBRARY_SOURCE_DIR}")


# executable creation
add_executable(libmgr_test libmgr_test.cpp)
#add_executable(client_test sync_client.cpp)

# libmgr_test linking
target_link_libraries(libmgr_test libmgr)

if(WIN32)
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		target_link_libraries(libmgr_test libboost_program_options-vc100-mt-gd-1_42)
	elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
		target_link_libraries(libmgr_test libboost_program_options-vc100-mt-1_42)
	endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
else(UNIX)
	target_link_libraries(libmgr_test libboost_program_options.so)
endif(WIN32)

add_dependencies(libmgr_test libmgr)

# client_test linking
#target_link_libraries(client_test libboost_system.so)
#target_link_libraries(client_test libboost_thread.so)

# add the install targets
install (TARGETS libmgr_test DESTINATION "${DOCLIBRARY_BINARY_DIR}/test/bin")
#install (TARGETS client_test libmgr_test DESTINATION "${DOCLIBRARY_BINARY_DIR}/test/bin")
