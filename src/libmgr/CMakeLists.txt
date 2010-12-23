#########################################################
# this is DocLibrary::libmgr cmake configuration file   #
#														#
# description : builds libmgr static library			#
#########################################################

# add_subdirectory (src/libmgr/tinyxml)
add_subdirectory (./tinyxml)

#add_library(libmgr STATIC doc_file_item.cpp virtual_folder.cpp doc_database.cpp)
add_library(libmgr doc_file_item.cpp virtual_folder.cpp doc_database.cpp)

# link with following libraries :
#	libtinyxml
target_link_libraries(libmgr tinyxml)
#	doclib::libcore
target_link_libraries(libmgr core)
#	boost::filesystem
#target_link_libraries(libmgr libboost_filesystem.a)
target_link_libraries(libmgr libboost_filesystem.so)

# explicitely tell about libmgr dependencies
add_dependencies(libmgr core)
add_dependencies(libmgr tinyxml)

install (TARGETS libmgr DESTINATION "${DOCLIBRARY_BINARY_DIR}/lib")
