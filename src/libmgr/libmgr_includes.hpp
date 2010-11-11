#ifndef DOCLIB_LIBMGR_LIB_MGR_INCLUDES_HPP
#define DOCLIB_LIBMGR_LIB_MGR_INCLUDES_HPP


// global headers
///////////////////////////////////////

#include <iostream>
#include <string>
//#include <map>
//#include <vector>
#include <list>
//#include <cassert>
#include <algorithm>
#include <exception>

// TinyXml Library
#define TIXML_USE_STL
#include "tinyxml/tinyxml.h"

// boost headers used everywhere in DocLibrary
///////////////////////////////////////

#include <boost/filesystem.hpp>
#include <boost/logging/logging.hpp>


// boost namespace aliases
///////////////////////////////////////

namespace bfs = boost::filesystem;
namespace bl = boost::logging;


#endif // !DOCLIB_LIBMGR_LIB_MGR_INCLUDES_HPP
