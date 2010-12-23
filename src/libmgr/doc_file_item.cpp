#include "doc_database.hpp"
#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

		doc_file_item_pointer doc_file_item::create(int id, string path, string filetype, virtual_folder_pointer sp_parent)
		{
			return doc_file_item_pointer(new doc_file_item(id, path, filetype, sp_parent));
		}


		doc_file_item::doc_file_item(int id, string path, string filetype, virtual_folder_pointer sp_parent)
            :	_id(id), _path(path), _filetype(filetype)
        {
			if (path == "/")
			{
				// old code : ugly with a const_cast
				//_sp_parent = const_cast<virtual_folder*>(static_cast<const virtual_folder*>(this));
				_sp_parent = virtual_folder_pointer(static_cast<virtual_folder*>(this));
			}
			else
			{
				//LDBG_ 	<< "id : " << id << ", path : " << path << ",filetype : " << filetype;
				assert(sp_parent.get() != 0);
				_sp_parent = sp_parent;
			}
			std::cout << "DBG : virtual_folder* pparent is = " << _sp_parent.get() << std::endl; 
		}

    }

}
