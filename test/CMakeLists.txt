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

# tell to link with libmgr
target_link_libraries(libmgr_test libmgr)
target_link_libraries(libmgr_test libboost_program_options.a)

# we depends on libmgr
add_dependencies(libmgr_test libmgr)

# add the install targets
install (TARGETS libmgr_test DESTINATION "${DOCLIBRARY_BINARY_DIR}/test/bin")
