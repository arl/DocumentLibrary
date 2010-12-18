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

		typedef std::tr1::shared_ptr<doc_file_item> sp_doc_file_item;
		typedef std::tr1::shared_ptr<virtual_folder> sp_virtual_folder;

        /** doc_file_item is abstract file represntation, can be a container of files
         *	or a concrete file residing on the filesystem
         */
        class doc_file_item
        {
            friend class doc_database;

            protected:

                doc_file_item(int id, string path, string filetype, sp_virtual_folder sp_parent);

                int _id;
                string _path;
                string _filetype;
                sp_virtual_folder _sp_parent;

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
				sp_virtual_folder get_parent() { return _sp_parent; }

				/**
				 * @brief return parent virtual folder (or itself for root) const version
				 *
				 * @return never null
				 */
				const sp_virtual_folder get_parent() const { return _sp_parent; }


		};

    }

}

#endif // !DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
