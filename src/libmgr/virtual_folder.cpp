#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

        int virtual_folder::_max_id = -1;

		virtual_folder::virtual_folder(string path, int id, virtual_folder * pparent)
			:	doc_file_item(id, path, "vfolder", pparent)
        {
            // root virtual folder must have -1 as id
            if (get_id() == -1)
                assert(_path == "/");

            // redefine _max_id
			_max_id = (id < _max_id) ? id : _max_id;
        }


        virtual_folder::virtual_folder(string path, virtual_folder * pparent)
            :	doc_file_item(--_max_id, path, "vfolder", pparent)
        {
            // root virtual folder must have -1 as id
            if (get_id() == -1)
                assert(_path == "/");
        }

        // add a child to this virtual folder
        void virtual_folder::add_child(doc_file_item * p_file)
        {
            _children.push_back(p_file);
        }

        // remove a child
        void virtual_folder::remove_child(int child_id)
        {
            file_list_t::iterator it = _children.begin();
            while(it != _children.end())
            {
                doc_file_item * p_cur = *it;
                if(p_cur->get_id() == child_id)
                {
                    _children.erase(it);
                    return;
                }
                ++it;
            }
        }
    }

}
