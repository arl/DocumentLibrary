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

# add the install targets
install (TARGETS doclibd DESTINATION "${DOCLIBRARY_BINARY_DIR}/bin")
