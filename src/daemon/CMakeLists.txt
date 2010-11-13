#################################################################
#																#
#	  doclibd (DocLibrary daemon) cmake configuration file		#
#																#
#################################################################

# include source code directory
include_directories("${DOCLIBRARY_SOURCE_DIR}")

# executable creation
add_executable(doclibd main.cpp)

# link with following libraries :
#	core
target_link_libraries(doclibd core)
#	library
target_link_libraries(doclibd libmgr)
#	boost::program_options
target_link_libraries(doclibd libboost_program_options.so)
#	boost::filesystem
target_link_libraries(doclibd libboost_filesystem.so)
#	libpthread
#target_link_libraries(doclibd libpthread.a)
#	libc
#target_link_libraries(doclibd libc.a)

# explicitely tell about doclibd dependencies
add_dependencies(doclibd libmgr)
add_dependencies(doclibd core)

# increment build number when doclibd target has been successfully built
add_custom_command(TARGET doclibd
				   POST_BUILD
				   WORKING_DIRECTORY "${DOCLIBRARY_SOURCE_DIR}/src/"
				   COMMAND ./increment_number.sh "core/version.cpp" "build" "core/version.cpp"
				   COMMENT "incrementing build number")

# add the install targets
install (TARGETS doclibd DESTINATION "${DOCLIBRARY_BINARY_DIR}/bin")
