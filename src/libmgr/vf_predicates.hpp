/* Copyright (C) 
 * 2011 - Aurelien Rainone
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

#ifndef DOCLIB_LIBMGR_VF_PREDICATES_HPP
#define DOCLIB_LIBMGR_VF_PREDICATES_HPP

#include "virtual_folder.hpp"
#include <list>

namespace doclib
{

	namespace libmgr
	{

		/**
		 * @brief predicate recording given doc_file_item pointers
		 *
		 * predicate recording each doc_file_item pointer passed as argument to
		 * operator () and put it in a list
		 */
		struct record_vf_values 
		{

			record_vf_values() {}

			record_vf_values(const record_vf_values& rhs) : _container(rhs._container)
			{
			}

			void operator () (const doc_file_item_pointer fp)
			{
				_container.push_back(fp);
			}

			/**
			 * @brief retrieve the container filled with passed values
			 *
			 * @return the container
			 */
			typedef std::list<doc_file_item_pointer> container_t;

			const container_t & get_them() const { return _container; }

			container_t _container;
		};



#if 0		
		/**
		 * @brief predicate recording doc_file_item values
		 *  
		 * records the values of each doc_file_item passed as argument to
		 * operator (). values are formatted according to a format string
		 * provided in record_vf_values constructor
		 */
		struct record_vf_values 
		{

			/** la format string can contain file and/or folder fields

example :  if (id was only if file) "(  ID  )" will be repalced by "  13  " for a file
and "" for a folder

example :  if (id was only if file) "(ab  ID  )" will be repalced by "ab  13  " for a file
and "" for a folder
*/



			enum e_record_what
			{
				record_folders	= 1,
				record_files	= 2,
				record_all		= record_folders | record_files
			};

			get_vf_values(int record_what std::string format_str)
				:	_format_str(format_str)
			{}

			void operator () (const virtual_folder_pointer vfp)
			{
				// to store intermediate replacements
				string tmp;

				// replace ID with file id
				boost::regex rx_id("ID");
				string file_id = "123";
				tmp = boost::regex_replace(format_str, rx_id, file_id);

				// replace NAME with file name
				boost::regex rx_name("NAME");
				string file_name = "filename.pdf";
				tmp = boost::regex_replace(tmp, rx_name, file_name);


				//std::string cur_str = "";
				//_result_str += _sep + cur	
				//_result_str.append(_sep);
				//_result_str.append(cur_str);
			}

			const std::string & get_result_str(std::string sep) const { return _result_str; }

			const std::string _file_format_str;
			const std::string _folder_format_str;
			std::vector<std::string> _values;
			std::string _result_str;
			int _record_what;

		};
#endif

	}

}
#endif // !DOCLIB_LIBMGR_VF_PREDICATES_HPP
