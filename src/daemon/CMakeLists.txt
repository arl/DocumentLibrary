#################################################################
#																#
#	  doclibd (DocLibrary daemon) cmake configuration file		#
#																#
#################################################################

# include source code directory
include_directories("${DOCLIBRARY_SOURCE_DIR}")

# executable creation
add_executable(doclibd connection.cpp reply.cpp request_parser.cpp request_handler.cpp server.cpp io_service_pool.cpp main.cpp)


# link with following libraries :
#	core
target_link_libraries(doclibd core)
#	library
target_link_libraries(doclibd libmgr)
#	boost::thread
target_link_libraries(doclibd libboost_thread.so)
#	boost::program_options
target_link_libraries(doclibd libboost_program_options.so)
#	boost::filesystem
target_link_libraries(doclibd libboost_filesystem.so)

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

# create list of configuration files
list(APPEND CONFIG_FILES "db.xml" "doclibd.conf")
set(CONFIG_DIR "${DOCLIBRARY_SOURCE_DIR}/config")

# install default configuration files is they don't exist on the system
foreach(file ${CONFIG_FILES})

	set(result "NOTFOUND")

	# look for config file in install dir
	find_file(result "${file}"
			PATHS "${DOCLIBRARY_BINARY_DIR}/bin"
			NO_DEFAULT_PATH)

	if(result STREQUAL "result-NOTFOUND")
		message("file \"${file}\" not found in installation dir, copying to ${DOCLIBRARY_BINARY_DIR}/bin")
		configure_file("${CONFIG_DIR}/${file}" "${DOCLIBRARY_BINARY_DIR}/bin/${file}" COPYONLY)
	endif(result STREQUAL "result-NOTFOUND")

endforeach(file)

