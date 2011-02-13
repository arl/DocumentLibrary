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

#ifndef DOCLIB_DOCLIB_INCLUDES_HPP 
#define DOCLIB_DOCLIB_INCLUDES_HPP 


// global headers
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <cassert>
#include <algorithm>
#include <exception>


// boost headers used everywhere in DocLibrary
#include <boost/filesystem.hpp>
#include <boost/logging/logging.hpp>

// doclibrary version definitions
#include "version.hpp"

// boost namespace aliases
namespace fs = boost::filesystem;
namespace bl = boost::logging;

#endif // !DOCLIB_DOCLIB_INCLUDES_HPP 
