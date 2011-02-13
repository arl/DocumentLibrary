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

#ifndef DOCLIB_LIBMGR_DOC_DATABASE_HPP
#define DOCLIB_LIBMGR_DOC_DATABASE_HPP

#include "libmgr_includes.hpp"
#include "virtual_folder.hpp"
#include "../core/doclib_conf.hpp"

namespace doclib
{

    namespace libmgr
    {
		
		using namespace std;


		/**
		* @brief document database singleton class
		*/
        class doc_database
        {

			// forward declaration
			class v_folder_visitor;
			friend class doc_file_item;
			friend class virtual_folder;

            public:

				/**
				* @brief create doc_database singleton instance
				*
				* @param conf main configuration file
				*/
				static void create_instance(const core::doclib_conf& conf);
				
				/**
				* @brief destroy doc_database singleton instance
				*/
				static void destroy_instance();

				/**
				* @brief get doc_database singleton instance
				*
				* @return the instance
				*/
				static doc_database& get_instance();

				/**
				* @brief load document database
				*
				* @note read virtual folder structure from xml then recreate this structure in
				*	memory
				* @return true on success
				*/
				bool load_database();

				/**
				* @brief get root virtual folder
				*
				* @return root virtual folder shared pointer
				*/
				static virtual_folder_pointer get_root_vf()
				{
					assert(p_instance);
					return p_instance->_sproot_vf;
				}

				/**
				* @brief retrieve a file item pointer based on its relative path from a
				*	virtual folder
				*
				* @param path path of file to retrieve
				* @param pvf_from relative root virtual folder
				*
				* @return the required file item pointer or an empty one if
				*	it could not be found
				*/
				doc_file_item_pointer get_file(std::string _path,
						virtual_folder_pointer pvf_from = get_root_vf());

				/**
				* @brief retrieve a file item pointer based on its unique id
				*
				* @param id unique id of file to retrieve
				*
				* @return the required file item pointer or an empty one if
				*	provided id doesn't exist
				*/
				doc_file_item_pointer get_file(int id);

            private:

				static doc_database* p_instance;

                doc_database(const core::doclib_conf& conf);

                ~doc_database();

                const core::doclib_conf& _conf;

				bool load_description_file();

				/// root virtual folder
				virtual_folder_pointer	_sproot_vf;		

				/// database root path
				string				_root_path;

				typedef std::map<int, doc_file_item_pointer> file_map_t;

				/// id/file dictionnary
				file_map_t _files;
        };

    }

}

#endif // !DOCLIB_LIBMGR_DOC_DATABASE_HPP
