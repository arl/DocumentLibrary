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

#ifndef DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
#define DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP

#include "libmgr_includes.hpp"
#include <boost/tr1/memory.hpp>


namespace doclib
{

    namespace libmgr
    {

		// forward declarations
		class virtual_folder;
		class doc_file_item;

		typedef std::tr1::shared_ptr<doc_file_item> doc_file_item_pointer;
		typedef std::tr1::shared_ptr<virtual_folder> virtual_folder_pointer;

        
		/**
		* @brief abstract file representation.
		*
		* doc_file_item is an abstract file representation, can be a file (typically
		*	a document of the library) or a file container (@see virtual_folder) 
		*/
        class doc_file_item
			: public std::tr1::enable_shared_from_this<doc_file_item>
        {
            friend class doc_database;

            protected:

				/**
				* @brief convenience function to create a doc_file_item shared pointer
				*
				* @param id file id read from xml configuration file
				* @param path file absolute path
				* @param filetype file type, should reflect extension for 'non text-files'
				* @param sp_parent virtual_folder parent
				*
				* @return the newly created doc_file_iteml shared pointer
				*/
				static doc_file_item_pointer create(int id, std::string path, std::string filetype, virtual_folder_pointer sp_parent);
				
				/**
				* @brief constructor for document file
				*
				* @param id file id read from xml configuration file
				* @param path file absolute path
				* @param path file extension string
				* @param sp_parent virtual_folder parent
				*/
				doc_file_item(int id, std::string path, std::string filetype, virtual_folder_pointer sp_parent);

            public:

				/**
				* @brief doc_file_item destructor
				*/
				virtual ~doc_file_item() {}

				/**
				* @brief retrieve file id
				*
				* @return file id, negative for virtual folders, positive for doc_file instances
				*/
                int get_id() const { return _id; }

				/**
				 * @brief retrieve file path (actual or virtual)
				 *
				 * with actual at true, get_path will return the filesystem path of the
				 *	file, with actual set at false the virtual path is returned.
				 *
				 * @note The virtual path being the path to access the file inside the document
				 *	library
				 *
				 * @param actual choose between :
				 *	<ul>
				 *	<li>true = actual path</li>
				 *	<li>false = virtual path</li>
				 *	</ul>
				 *
				 * @return file path, in an absolute form
				 */
                virtual std::string get_path(bool actual = true) const;

				/**
				* @brief retrieve file name
				*
				* @return file name string
				*/
                virtual std::string get_name() const { return _name; }

				/**
				 * @brief retrieve file type
				 *
				 * @note file type inconsistent with extension should be avoided
				 *
				 * @return file type string
				 */
                std::string get_filetype() const { return _filetype; }

				/**
				* @brief return parent virtual folder (or itself for root)
				*
				* @return never null
				*/
				virtual_folder_pointer get_parent() { return _sp_parent; }

				/**
				 * @brief return parent virtual folder (or itself for root), const version
				 *
				 * @return never null
				 */
				const virtual_folder_pointer get_parent() const { return _sp_parent; }

			protected:

				
				/**
				* @brief file unique identifier
				*	
				*	<ul>
				*	<li>negative for virtual folders (root is <b>always</b> -1)</li>
				*	<li>positive for files</li>
				*	</ul>
				*/
                int _id;

				/// represents actual file system path (or name for virtual folders)
				std::string _path;

				/// file name
                std::string _name;

				/// extension for files, "vfolder" for virtual folders
                const std::string _filetype;

				/// shared pointer to parent folder
                virtual_folder_pointer _sp_parent;


		};

    }

}

#endif // !DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
