#########################################################
# this is DocLibrary::libmgr cmake configuration file   #
#														#
# description : builds libmgr static library			#
#########################################################

add_definitions(-DTIXML_USE_STL)
add_library(libtinyxml tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp)

install (TARGETS libtinyxml DESTINATION "${DOCLIBRARY_BINARY_DIR}/lib")
