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

#ifndef DOCLIB_DAEMON_DOCLIBD_HPP
#define DOCLIB_DAEMON_DOCLIBD_HPP

#include <string>

namespace doclib
{

	namespace daemon
	{
		enum e_command_line_treatment
		{
			clt_success,	/// command-line treated with success
			clt_failure,	/// failed to treat command line options
			clt_help		///	help asked
		};
		
		struct commandline_options
		{
			e_command_line_treatment treatment;
			std::string config_file;
		};

		void treat_command_line(int argc, char ** argv, commandline_options & options);

		const std::string get_version_string();

	}

}

#endif // !DOCLIB_DAEMON_DOCLIBD_HPP
