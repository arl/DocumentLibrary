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

#ifndef DOCLIB_CORE_DOCLIB_EXCEPTION_HPP
#define DOCLIB_CORE_DOCLIB_EXCEPTION_HPP

#include "core_includes.hpp"

namespace doclib
{

	namespace core
	{

		using namespace std;


		class doclib_exception : public std::exception
		{
			public:

				doclib_exception(string msg)
				{
					_msg = "exception thrown : ";
					_msg.append(msg);
					log_exception();
				}

				virtual ~doclib_exception() throw()
				{
				}

				virtual const char* what() const throw()
				{
					return _msg.c_str();
				}

			private:

				void log_exception()
				{
					LERR_ << "exception thrown : " << _msg;
				}

				string _msg;

		};

	}

}

#endif // !DOCLIB_CORE_DOCLIB_EXCEPTION_HPP
