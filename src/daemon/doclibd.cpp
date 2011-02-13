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

#include "doclibd_includes.hpp"
#include "doclibd.hpp"
#include "../version.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>

namespace doclib
{

	namespace daemon
	{

		void treat_command_line(int argc, char ** argv, commandline_options & options)
		{
			// Declare the supported options.
			po::options_description desc("Allowed options");
			desc.add_options()
				("help,h", "produce help message")
				("config-file,c", po::value<std::string>(), "set configuration from filename instead")
				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			if (vm.count("help"))
			{
				options.treatment = clt_help;
				std::cout << "DocLibrary version " << get_version_string() << std::endl;
				std::cout << desc << std::endl;
				options.treatment = clt_failure;
				return;
			}

			if (vm.count("config-file"))
			{
				std::cout << "using "
					<< vm["config-file"].as<std::string>() << " as configuration file." << std::endl;
				options.config_file = vm["config-file"].as<std::string>();
			} else {
				// set default configuration file
				options.config_file = std::string(DOCLIBD_DEFAULT_CFG_FILENAME);
			}

			options.treatment = clt_success;
			return;
		}

		const std::string get_version_string()
		{
			std::string version_str;
			version_str.assign(boost::lexical_cast<std::string>(doclib::version::major));
			version_str.append(".");
			version_str.append(boost::lexical_cast<std::string>(doclib::version::minor));
			version_str.append(".");
			version_str.append(boost::lexical_cast<std::string>(doclib::version::build));
			return version_str;
		}

	}

}
