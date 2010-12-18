#ifndef DOCLIB_LIBMGR_VIRTUAL_FOLDER_HPP
#define DOCLIB_LIBMGR_VIRTUAL_FOLDER_HPP

#include "doc_file_item.hpp"

namespace doclib
{

    namespace libmgr
    {
        // forward declarations
        class doc_database;

        using namespace std;

        class virtual_folder : public doc_file_item
        {
            friend class doc_database;

            private:

				/**
				* @brief constructor for a user-created virtual folder
				* @note user created folders have no id yet, their id will be set
				*	as _max_id -1
				* @param path name of the virtual_folder
				*/
                virtual_folder(string path, sp_virtual_folder sp_parent);

				/**
				* @brief 'xml file-created' virtual folder
				*
				* @param path name of virtual folder
				* @param id virtual folder id indicated in xml file
				*/
                virtual_folder(string path, int id, sp_virtual_folder sp_parent);

                static int _max_id;

            protected:

                typedef list<sp_doc_file_item> file_list_t;

                file_list_t _children;

            public:

                // add a child to this virtual folder
                void add_child(sp_doc_file_item sp_file);

                // remove a child
                void remove_child(int child_id);

                // call some function for each child
                template<class _Pred>
                    void for_each_child(_Pred p, bool recursive = false, bool pred_called_last = false)
                {
                    file_list_t::iterator it = _children.begin();
                    while(it != _children.end())
                    {
                        sp_doc_file_item sp_cur = *it;

                        // call predicate on current child (default : before recursion)
						if (!pred_called_last) 
							p(sp_cur);

                        // recurse in this virtual folder
                        if (recursive && sp_cur->get_id() < 0)
                        {
                            static_cast<virtual_folder*>(sp_cur.get())->for_each_child(p, true);
                        }

                        // call predicate on current child (after recursion)
						if (pred_called_last) 
							p(sp_cur);

                        ++it;
                    }
                }

    	};

    }

}
#endif // !DOCLIB_LIBMGR_VIRTUAL_FOLDER_HPP
