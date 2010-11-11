#ifndef DOCLIB_CORE_INCLUDES_HPP
#define DOCLIB_CORE_INCLUDES_HPP


// global headers
///////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <map>
//#include <vector>
//#include <list>
//#include <cassert>
//#include <algorithm>
#include <exception>


// boost headers used everywhere in DocLibrary
///////////////////////////////////////

#include <boost/filesystem.hpp>
#include <boost/logging/format_fwd.hpp>


// boost namespace aliases
///////////////////////////////////////

namespace fs = boost::filesystem;
namespace bl = boost::logging;


// core library definitions
///////////////////////////////////////
#define DAEMON_MUTEX_FILENAME "/tmp/doclibd_lock"


// logger declaration
///////////////////////////////////////
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> ) 

// declare logging class, filters and logger instance
typedef boost::logging::logger_format_write< > log_type;
BOOST_DECLARE_LOG_FILTER(g_l_filter, bl::level::holder)
BOOST_DECLARE_LOG(g_l, log_type)

// log macros
// LDBG_ will log if log_level is set to debug => will log about anything
#define LDBG_	BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), debug ) << "[DBG] "
// LERR_ will log if log_level is set to debug or error => log software crashes/errors
#define LERR_	BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), error ) << "[ERR] "
// L_ will always log, independently from log level
#define L_		BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info )

// Useful type definitions
typedef std::map<std::string, std::string>NameValuePairList;

#endif // !DOCLIB_CORE_INCLUDES_HPP
