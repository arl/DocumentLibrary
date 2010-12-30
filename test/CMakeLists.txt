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
add_executable(client_test sync_client.cpp)

# libmgr_test linking
target_link_libraries(libmgr_test libmgr)
target_link_libraries(libmgr_test libboost_program_options.so)
add_dependencies(libmgr_test libmgr)

# client_test linking
target_link_libraries(client_test libboost_system.so)
target_link_libraries(client_test libboost_thread.so)

# add the install targets
install (TARGETS client_test libmgr_test DESTINATION "${DOCLIBRARY_BINARY_DIR}/test/bin")
