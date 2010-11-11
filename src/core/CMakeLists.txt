#########################################################
# this is DocLibrary::core cmake configuration file		#
#														#
# description : builds libcore static library			#
#########################################################

#add_library(core STATIC doclib_conf.cpp doclib_core.cpp)
add_library(core inifile.cpp doclib_conf.cpp doclib_core.cpp)

# link with following libraries :
#	boost::system
target_link_libraries(core libboost_system.so)
#	boost::filesystem
target_link_libraries(core libboost_filesystem.so)

install (TARGETS core DESTINATION "${DOCLIBRARY_BINARY_DIR}/lib")
