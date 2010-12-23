#ifndef DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
#define DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP

#include "libmgr_includes.hpp"
#include <boost/tr1/memory.hpp>


namespace doclib
{

    namespace libmgr
    {

        using namespace std;

		// forward declarations
		class virtual_folder;
		class doc_file_item;

		typedef std::tr1::shared_ptr<doc_file_item> doc_file_item_pointer;
		typedef std::tr1::shared_ptr<virtual_folder> virtual_folder_pointer;

        /** doc_file_item is an abstract file representation, can be a file (typically a document of the library)
         *	or be a file container (virtual folder)
         */
        class doc_file_item
        {
            friend class doc_database;

            protected:

				/**
				* @brief convenience function to create a doc_file_item shared pointer
				* @param id file id read from xml configuration file
				* @param path name of the virtual_folder
				* @param filetype file type, should reflect extension for 'non text-files'
				* @param sp_parent virtual_folder parent
				*/
				static doc_file_item_pointer create(int id, string path, string filetype, virtual_folder_pointer sp_parent);
				
				/**
				* @brief constructor for document file
				* @param id file id read from xml configuration file
				* @param path name of the virtual_folder
				* @param sp_parent virtual_folder parent
				*/
				doc_file_item(int id, string path, string filetype, virtual_folder_pointer sp_parent);

                int _id;
                string _path;
                string _filetype;
                virtual_folder_pointer _sp_parent;

            public:

				/**
				* @brief retrieve file id
				*
				* @return file id, negative for virtual folders, positive for doc_file instances
				*/
                int get_id() const { return _id; }

				/**
				 * @brief retrieve file path
				 *
				 * @return file path, in an absolute form
				 */
                string get_path() const { return _path; }

				/**
				 * @brief retrieve file type
				 *
				 * @return file type
				 * @note file type inconsistent with extension should be avoided
				 */
                string get_filetype() const { return _path; }

				/**
				* @brief return parent virtual folder (or itself for root)
				*
				* @return never null
				*/
				virtual_folder_pointer get_parent() { return _sp_parent; }

				/**
				 * @brief return parent virtual folder (or itself for root) const version
				 *
				 * @return never null
				 */
				const virtual_folder_pointer get_parent() const { return _sp_parent; }


		};

    }

}

#endif // !DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
