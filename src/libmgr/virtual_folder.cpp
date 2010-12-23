#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

		virtual_folder_pointer virtual_folder::create(string path, virtual_folder_pointer sp_parent)
		{
			return virtual_folder_pointer(new virtual_folder(path, sp_parent));
		}

		virtual_folder_pointer virtual_folder::create(string path, int id, virtual_folder_pointer sp_parent)
		{
			return virtual_folder_pointer(new virtual_folder(path, id, sp_parent));
		}

        int virtual_folder::_max_id = -1;

		virtual_folder::virtual_folder(string path, int id, virtual_folder_pointer sp_parent)
			:	doc_file_item(id, path, "vfolder", sp_parent)
        {
            // root virtual folder must have -1 as id
            if (get_id() == -1)
                assert(_path == "/");

            // redefine _max_id
			_max_id = (id < _max_id) ? id : _max_id;
        }


		virtual_folder::virtual_folder(string path, virtual_folder_pointer sp_parent)
            :	doc_file_item(--_max_id, path, "vfolder", sp_parent)
        {
			assert(_path != "/");
        }

        // add a child to this virtual folder
        void virtual_folder::add_child(doc_file_item_pointer sp_file)
        {
            _children.push_back(sp_file);
        }

        // remove a child
        void virtual_folder::remove_child(int child_id)
        {
            file_list_t::iterator it = _children.begin();
            while(it != _children.end())
            {
                doc_file_item_pointer sp_cur = *it;
                if(sp_cur->get_id() == child_id)
                {
                    _children.erase(it);
                    return;
                }
                ++it;
            }
        }

	}

}
