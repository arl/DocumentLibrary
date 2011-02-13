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

#ifndef DOCLIB_CORE_DOCLIB_CORE_HPP
#define DOCLIB_CORE_DOCLIB_CORE_HPP

#include "core_includes.hpp"
#include "doclib_conf.hpp"

namespace doclib
{

	namespace core
	{

        using namespace std;

		class core_services
		{
			public:

				/**	create core_services singleton instance.
				  @note
					  (will throw if already exists)
				*/
				static void create();


				/** checks if core_services singleton already exists or not.
				*/
				static bool exists();

				/** get core_services singleton instance.
				*/
				static core_services& get();

				/** init logging system.
				  @note
					  (will throw if instances doesn't exist)
				 */
				static void init_logging(const doclib_conf & conf);

			private:

				/// private constructor.
				core_services();

				/// singleton instance.
				static core_services * _inst;

		};

	}

}

#endif // !DOCLIB_CORE_DOCLIB_CORE_HPP
