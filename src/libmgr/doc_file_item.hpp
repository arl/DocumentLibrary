#ifndef DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
#define DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP

#include "libmgr_includes.hpp"

namespace doclib
{

    namespace libmgr
    {

        using namespace std;

		class virtual_folder;


        /** doc_file_item is abstract file represntation, can be a container of files
         *	or a concrete file residing on the filesystem
         */
        class doc_file_item
        {
            protected:

                doc_file_item(int id, string path, string filetype, virtual_folder * pparent);

                int _id;
                string _path;
                string _filetype;
                virtual_folder * _pparent;

            public:

				/**
				* @brief get file id
				*
				* @return file id, negative for virtual folders, positive for doc_file instances
				*/
                int get_id() const { return _id; }

				/**
				* @brief return parent virtual folder (or itself for root)
				*
				* @return never null
				*/
				virtual_folder * get_parent() { return _pparent; }

				/**
				 * @brief return parent virtual folder (or itself for root) const version
				 *
				 * @return never null
				 */
				const virtual_folder * get_parent() const { return _pparent; }


		};

    }

}

#endif // !DOCLIB_LIBMGR_DOC_FILE_ITEM_HPP
