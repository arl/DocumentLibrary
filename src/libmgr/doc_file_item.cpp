#include "virtual_folder.hpp"
//#include "doc_file_item.hpp"

namespace doclib
{

    namespace libmgr
    {

        doc_file_item::doc_file_item(int id, string path, string filetype, virtual_folder * pparent)
            :	_id(id), _path(path), _filetype(filetype)
        {
			if (path == "/")
			{
				_pparent = const_cast<virtual_folder*>(static_cast<const virtual_folder*>(this));
			}
			else
			{
				assert(pparent);
				_pparent = pparent;
			}
			std::cout << "virtual_folder* pparent is = " << _pparent << std::endl; 
		}

    }

}
