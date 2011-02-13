/* Copyright (C) 
* 2010 - Aurelien Rainone
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

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

namespace bfs = boost::filesystem;
namespace bl = boost::logging;


// core library definitions
///////////////////////////////////////
/// @todo DAEMON_MUTEX should be user configurable (put it in config file)
#define DAEMON_MUTEX_FILENAME "/tmp/doclibd_lock"


// logger declaration
///////////////////////////////////////
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> ) 

// declare logging class, filters and logger instance
typedef boost::logging::logger_format_write < > log_type;
BOOST_DECLARE_LOG_FILTER(g_l_filter, bl::level::holder)
BOOST_DECLARE_LOG(g_l, log_type)

// log macros
// LDBG_ will only log if log_level is set to debug => used for debug purposes
#define LDBG_	BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), debug ) << "[DBG] "

// L_ will log if log level is set to debug or info => used for warnings
#define L_		BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info ) << " "

// LERR_ will always log independently from log level => used for crashes/errors
#define LERR_	BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), error ) << "[ERR] "

// Useful type definitions
typedef std::map <std::string, std::string> NameValuePairList;

#endif // !DOCLIB_CORE_INCLUDES_HPP
