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

#include "doc_database.hpp"
#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

		doc_file_item_pointer doc_file_item::create(int id, std::string path, std::string filetype, virtual_folder_pointer sp_parent)
		{
			doc_file_item_pointer spf (new doc_file_item(id, path, filetype, sp_parent));

			// register it into doc_database file map
			doc_database::get_instance()._files.insert(std::make_pair(id, spf));
			LDBG_ << "inserting " << spf->get_path() << "[" << spf->get_id() << "] in doc_db._files";
			LDBG_ << "now doc_db files has " << doc_database::get_instance()._files.size() << " elements";
			return spf;
		}

		doc_file_item::doc_file_item(int id, std::string path, std::string filetype, virtual_folder_pointer sp_parent)
            :	_id(id), _path(path), _filetype(filetype)
        {
			if (_filetype != "vfolder")
			{
				bfs::path file_path(_path) ;
				_name.assign(file_path.filename());
			}
			else
				// virtual folder name and path are undistinct
				_name.assign(path);

			if (_name == "/")
			{
				assert(_filetype == "vfolder");
				_sp_parent = virtual_folder_pointer(static_cast<virtual_folder*>(this));
			}
			else
			{
				assert(sp_parent.get() != 0);
				_sp_parent = sp_parent;
			}
			std::cout << "DBG : virtual_folder* pparent is = " << _sp_parent.get() << std::endl; 
		}

		std::string doc_file_item::get_path(bool actual /*= true*/) const
		{
			std::string path;

			if (actual)
			{
				// return filesystem path
				path = _path;
			}
			else
			{
				// otherwise we want the virtual path
				path.assign(get_parent()->get_path());
				if (path != "/")
					path.append("/");
				path.append(get_name());
			}
			return path;
		}


    }

}
