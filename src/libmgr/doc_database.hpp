#ifndef DOCLIB_LIBMGR_DOC_DATABASE_HPP
#define DOCLIB_LIBMGR_DOC_DATABASE_HPP

#include "libmgr_includes.hpp"
#include "../core/doclib_conf.hpp"

namespace doclib
{

    namespace libmgr
    {
		
		using namespace std;

		// forwafrd declaration
		class virtual_folder;


        class doc_database
        {

			// forward declaration
			class v_folder_visitor;

            public:

				
				/**
				* @brief create doc_database singleton instance
				*
				* @param conf main configuration file
				*/
				static void create_instance(const core::doclib_conf& conf);

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
				* @return root virtual folder pointer
				*/
				virtual_folder * get_root_vf() { return _proot_vf;}

            private:

				static doc_database* p_instance;

                doc_database(const core::doclib_conf& conf);

                const core::doclib_conf& _conf;

				bool load_description_file();

				virtual_folder *	_proot_vf;		// root virtual folder
				string				_root_path;		// database root path
        };

    }

}

#endif // !DOCLIB_LIBMGR_DOC_DATABASE_HPP
