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
				* @brief convenience function to create a virtual folder shared pointer
				*
				* @note user created folders have no id yet, their id will be set
				*	as _max_id -1
				*
				* @param name name of the virtual_folder
				* @param sp_parent virtual_folder parent
				*
				* @return the newly created virtual folder shared pointer
				*/
				static virtual_folder_pointer create(string name, virtual_folder_pointer sp_parent);

				/**
				* @brief convenience function for 'xml file-created' virtual folder
				*
				* @note user created folders have no id yet, their id will be set
				*	as _max_id -1
				*
				* @param name name of virtual folder
				* @param id virtual folder id read from xml configuration file
				* @param sp_parent virtual_folder parent
				*
				* @return the newly created virtual folder shared pointer
				*/
				static virtual_folder_pointer create(string name, int id, virtual_folder_pointer sp_parent);

				/**
				* @brief constructor for a user-created virtual folder
				*
				* @note user created folders have no id yet, their id will be set
				*	as _max_id -1
				*
				* @param name name of the virtual_folder
				* @param sp_parent virtual_folder parent
				*/
                virtual_folder(string name, virtual_folder_pointer sp_parent);

				/**
				* @brief 'xml file-created' virtual folder
				*
				* @param name name of virtual folder
				* @param id virtual folder id indicated in xml file
				* @param sp_parent virtual_folder parent
				*/
                virtual_folder(string name, int id, virtual_folder_pointer sp_parent);

                static int _max_id;

            protected:

                typedef list<doc_file_item_pointer> file_list_t;

                file_list_t _children;

            public:

				/**
				 * @name children operations
				 * @{ */

				/**
				* @brief add a child to a virtual folder
				*
				* @param sp_file file_item shared pointer to add
				*/
                void add_child(doc_file_item_pointer sp_file);

				/**
				* @brief remove an existing child from a virtual folder
				*
				* @param child_id id of the child to remove
				*/
                void remove_child(int child_id);

				/**
				 * @brief call a function on each children of a virtual folder 
				 *
				 * @tparam _Pred type of the predicate function to call
				 *
				 * @param p function to call
				 * @param recursive call recursively on each children
				 * @param depth_first if recursive is true, __pred is recursively called on
				 *	all children, starting from the deepest one (<b>depth_first</b>)
				 *
				 * @return the predicate reference in case of you have to access
				 *	a member modified during the call
				 */
					template<typename _Pred>
				_Pred for_each_child(_Pred __pred, bool recursive = false, bool depth_first = true)
				{
					file_list_t::iterator it = _children.begin();
					while(it != _children.end())
					{
						doc_file_item_pointer sp_cur = *it;

						// call predicate on current child (default : before recursion)
						if (!depth_first) 
							__pred(sp_cur);

						// recurse in this virtual folder
						if (recursive && sp_cur->get_id() < 0)
							static_cast<virtual_folder*>(sp_cur.get())->for_each_child(__pred, true);

						// call predicate on current child (after recursion)
						if (depth_first) 
							__pred(sp_cur);

						++it;
					}
					return __pred;
				}

				/**  @} */

				/**
				 * @brief retrieve virtual folder path
				 *
				 * for a virtual folder, the path represents the absolute path
				 *	from document database root to get to this current virtual
				 *	folder
				 *
				 * @param actual has no effect for a virtual folder
				 *
				 * @return absolute virtual folder path from document database root
				 */
                virtual std::string get_path(bool actual = true) const;

				/**
				* @brief retrieve virtual folder name
				*
				* @return folder name string
				*/
                virtual std::string get_name() const { return _name; }

		};

	}

}
#endif // !DOCLIB_LIBMGR_VIRTUAL_FOLDER_HPP
