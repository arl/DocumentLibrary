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

#include "virtual_folder.hpp"
#include "doc_database.hpp"
#include "../core/core_includes.hpp"

namespace doclib
{

    namespace libmgr
    {

		virtual_folder_pointer virtual_folder::create(string name, virtual_folder_pointer sp_parent)
		{
			virtual_folder_pointer spf(new virtual_folder(name, sp_parent));

			// register it into doc_database file map
			doc_database::get_instance()._files.insert(std::make_pair(spf->get_id(), spf));
			LDBG_ << "inserting " << spf->get_path() << "[" << spf->get_id() << "] in doc_db._files";
			LDBG_ << "now doc_db files has " << doc_database::get_instance()._files.size() << " elements";
			return spf;
		}

		virtual_folder_pointer virtual_folder::create(string name, int id, virtual_folder_pointer sp_parent)
		{
			virtual_folder_pointer spf(new virtual_folder(name, id, sp_parent));

			// register it into doc_database file map
			doc_database::get_instance()._files.insert(std::make_pair(id, spf));
			LDBG_ << "inserting " << spf->get_path() << "[" << spf->get_id() << "] in doc_db._files";
			LDBG_ << "now doc_db files has " << doc_database::get_instance()._files.size() << " elements";
			return spf;
		}

        int virtual_folder::_max_id = -1;

		virtual_folder::virtual_folder(string name, int id, virtual_folder_pointer sp_parent)
			:	doc_file_item(id, name, "vfolder", sp_parent)
        {
            // root virtual folder must have -1 as id
            if (get_id() == -1)
                assert(_name == "/");

            // redefine _max_id
			_max_id = (id < _max_id) ? id : _max_id;
        }


		virtual_folder::virtual_folder(string name, virtual_folder_pointer sp_parent)
            :	doc_file_item(--_max_id, name, "vfolder", sp_parent)
        {
			assert(_name != "/");
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

		std::string virtual_folder::get_path(bool actual /*= true*/) const
		{
			if (_name == "/")
				return _name;

			std::string path(get_parent()->get_path());
			if (path != "/")
				path.append("/");
			path.append(_name);
			return path;
		}
/*
doc_file_item :
	name -> file name
	path -> actual = on filesystem
			virtual = on doc db

virtual_folder :
	name -> folder name
	path -> actual = virtual = on doc db

*/

	}

}
