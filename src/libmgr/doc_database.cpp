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

		/**
		* @brief information printing predicate for file_item
		* @note	used for debugging purposes
		*/
		struct print_file_item
		{
			print_file_item() : indent(0) {}

			int indent;
	
			void operator () (doc_file_item_pointer spfi)
			{
				//if (pfi->get_file_type() == "vfolder")
				//{
					//std::cout << "virtual folder (" << pfi->get_path() << ", " << pfi->get_id() << ")" << std::endl;
					L_ << spfi->get_filetype()  << " : (" << spfi->get_path() << ", " << spfi->get_id() << ")" ;
				//}

			}
		};


		/**
		* @brief virtual folder visitor for xml vi_folder description file
		*/
		class doc_database::v_folder_visitor : public TiXmlVisitor
		{
			private:

				ostream & out;
				virtual_folder_pointer _sproot_vf;
				virtual_folder_pointer _spcur_vf;

				// different visitor states
				enum { eVirtualFolder, eChildren, eFile };

				// database root path
				string _root_path;	

				// represents the visitor state
				int _state;

				string 	_last_text;
				bool 	_last_is_relative;
				int 	_last_id;

			public:

				v_folder_visitor(string root_path)
					:	_root_path(root_path),
						_state(eVirtualFolder),
						out(cout) {}

				virtual ~v_folder_visitor() {}


				virtual_folder_pointer get_root_vf() { return _sproot_vf; }
				

				/// Visit an element.
				virtual bool VisitEnter( const TiXmlElement& elt, const TiXmlAttribute* /*first_attr*/)
				{
					out << "element::VisitEnter " << elt.Value() << endl;

					string tag_name = elt.Value();
					if (tag_name == "virtual_folder")
					{
						string name = elt.Attribute("name");
						int id;
						if (TIXML_SUCCESS != elt.QueryIntAttribute("id", &id))
						{
							LERR_ << "id attribute of <virtual_folder> is not an integer (line " << elt.Row() << ")";
							return false;	
						}
						if (id >= 0)
						{
							LERR_ << "id attribute of <virtual_folder> must be negative (line " << elt.Row() << ")";
							return false;	
						}
						if (name.empty())
						{
							LERR_ << "name attribute of <virtual_folder> can't be empty (line " << elt.Row() << ")";
							return false;	
						}
	
						// check if this is root vf
						if (name == "/")
						{
							// actually create root virtual folder
							virtual_folder_pointer sp_empty;	

							// let's create the root virtual folder shared pointer
							_spcur_vf = _sproot_vf = virtual_folder::create (name, -1, sp_empty);

							//LDBG_ << "allocating root at address " << lexical_cast<string>(_sproot_vf.get());

							LDBG_ << "virtual_folder(root) : ";
							LDBG_ << "    id       : " << _spcur_vf->get_id();
							LDBG_ << "    path(act): " << _spcur_vf->get_path();
							LDBG_ << "    path(vir): " << _spcur_vf->get_path(false);
							LDBG_ << "    name     : " << _spcur_vf->get_name();
							LDBG_ << "    filetype : " << _spcur_vf->get_filetype() << std::endl;
						}
						else
						{
							// create a new virtual folder
							virtual_folder_pointer sp_new_vf = virtual_folder::create (name, id, _spcur_vf);
							//LDBG_ << "allocating " << sp_new_vf->get_path()  << " at address " << lexical_cast<string>(sp_new_vf.get());
							LDBG_ << "virtual_folder : ";
							LDBG_ << "    id       : " << sp_new_vf->get_id();
							LDBG_ << "    path(act): " << sp_new_vf->get_path();
							LDBG_ << "    path(vir): " << sp_new_vf->get_path(false);
							LDBG_ << "    name     : " << sp_new_vf->get_name();
							LDBG_ << "    filetype : " << sp_new_vf->get_filetype() << std::endl;

							// add it to current vf
							_spcur_vf->add_child(sp_new_vf);

							// now let it be the current virtual folder for next processing
							_spcur_vf = sp_new_vf;
						}
					}
					else if (tag_name == "children")
					{
					
					}
					else if (tag_name == "file")
					{
						int id;
						if (TIXML_SUCCESS != elt.QueryIntAttribute("id", &id))
						{
							LERR_ << "id attribute of <file> is not an integer (line " << elt.Row() << ")";
							return false;	
						}
						if (id < 0)
						{
							LERR_ << "id attribute of <file> must be strictly positive (line " << elt.Row() << ")";
							return false;	
						}
						_last_id = id;
					}
					else if (tag_name == "path")
					{
						int is_rel;
						if (TIXML_SUCCESS != elt.QueryIntAttribute("relative", &is_rel))
						{
							LERR_ << "relative attribute of <path> is not an integer (line " << elt.Row() << ")";
							return false;	
						}
						if (is_rel != 0 && is_rel != 1)
						{
							LERR_ << "relative attribute of <path> must be 1 or 0 (line " << elt.Row() << ")";
							return false;	
						}
						_last_is_relative = (is_rel == 1);
					}

					return true;

				}

				/// Visit an element.
				virtual bool VisitExit( const TiXmlElement& elt )
				{
					out << "element::VisitExit " << elt.Value() << endl;

					string tag_name = elt.Value();
					if (tag_name == "virtual_folder")
					{
						// change back current virtual folder	
						if (elt.Attribute("name") != "/")
						{
							// current virtual folder's parent become the current one
							_spcur_vf = _spcur_vf->get_parent();
						}
					}
					else if (tag_name == "children")
					{
					
					}
					else if (tag_name == "file")
					{
						bfs::path file;
						// compute path from root_path
						if (_last_is_relative)
							file = bfs::path(_root_path) / _last_text;
						// absolute path
						else
							file = bfs::path(_last_text);

						// check for file existence and issue a warning in case
						if (!bfs::exists(file))
							L_ << "file \"" << file.string() << "\", id:" << _last_id  << " not found";
						
						// create the file with information obtained previously
						doc_file_item_pointer spf = doc_file_item::create(_last_id, file.string(), file.extension(), _spcur_vf);

						LDBG_ << "doc_file_item : ";
						LDBG_ << "    id       : " << spf->get_id();
						LDBG_ << "    path(act): " << spf->get_path();
						LDBG_ << "    path(vir): " << spf->get_path(false);
						LDBG_ << "    name     : " << spf->get_name();
						LDBG_ << "    filetype : " << spf->get_filetype() << std::endl;

						// add it as a child to current vf
						_spcur_vf->add_child(spf);
					}
					else if (tag_name == "path")
					{

					
					}


					return true;
				}

				/// Visit a text node
				virtual bool Visit( const TiXmlText& text )	
				{
					// record text content
					_last_text = text.Value();
					// dbg
					out << "text::Visit " << _last_text << endl;
					return true;
				}
		};

		// singleton instance
		doc_database * doc_database::p_instance = 0;

		void doc_database::create_instance(const core::doclib_conf & conf)
		{
			assert(p_instance == 0);
			p_instance = new doc_database(conf);
			LDBG_ << "doc_database instance created at " << (void*)p_instance;
		}

		void doc_database::destroy_instance()
		{
			assert(p_instance != 0);
			delete p_instance;
			p_instance = 0;
			LDBG_ << "doc_database instance destroyed";
		}

		doc_database& doc_database::get_instance()
		{
			assert(p_instance);
			return *p_instance;
		}

        doc_database::doc_database(const core::doclib_conf & conf)
            :	_conf(conf)
        {
        }

        doc_database::~doc_database()
        {
			// free virtual folders and their content

			// nomore need for this, shared_pointers auto deallocates
			//free_file_item __pred;
			//_proot_vf->for_each_child(__pred, true, true);
        }

		bool doc_database::load_database()
		{
			if (load_description_file())
			{
				// TODO CONTINUE HERE
				// TODO CONTINUE HERE
				// TODO CONTINUE HERE
				// TODO CONTINUE HERE
				return true;
			}
			LERR_ << "document database description file not found \"" << _conf.get_db_description_file() << "\"";
			return false;
		}

		bool doc_database::load_description_file()
		{
			bool ret = false;

			TiXmlDocument doc;
			if (bfs::exists(_conf.get_db_description_file()))
			{
				if (doc.LoadFile(_conf.get_db_description_file()))
				{
					TiXmlHandle hdoc(&doc);

					// get <folder_structure> tag
					TiXmlElement * pfold_str = hdoc.FirstChild("folder_structure").ToElement();
					if (pfold_str)
					{
						TiXmlHandle hfold_str(pfold_str);

						// get <root_path> tag
						TiXmlElement * proot_path = hfold_str.FirstChild("root_path").ToElement();
						// get root <virtual_folder> tag
						TiXmlElement * pvirt_fold = hfold_str.FirstChild("virtual_folder").ToElement();
						// check existence of both necessary tags
						if (proot_path && pvirt_fold)
						{
							// get database root path
							_root_path = proot_path->GetText();
				
							// fill database files and v folders with a XML visitor
							v_folder_visitor * pvisitor = new v_folder_visitor(_root_path);
							pvirt_fold->Accept(pvisitor);

							// retrieve root virtual folder from visitor
							_sproot_vf = pvisitor->get_root_vf();
							delete pvisitor;	

							// DBG
							print_file_item __pred;
							_sproot_vf->for_each_child(__pred, true);

							ret = true;
						}
						else
							LERR_ << "syntax error in document database description \"" << _conf.get_db_description_file() << "\"" << " no <root_path> and/or root <virtual folder> tag found";

					}
					else
						LERR_ << "syntax error in document database description \"" << _conf.get_db_description_file() << "\"" << " no <folder_structure> tag found";
				}
				else
					LERR_ << "can not load document database description \"" << _conf.get_db_description_file() << "\"" << doc.ErrorDesc();
			}

			return ret;
		}

		struct find_child_with_name
		{
			find_child_with_name(std::string name_to_find) : _name_to_find(name_to_find), _found_id(0) {}

			void operator() (const doc_file_item_pointer pvf)
			{
				if (pvf->get_name() == _name_to_find)
					_found_id = pvf->get_id();
			}
			int _found_id;
			const std::string _name_to_find;
		};

/*
	
	chemin complet : /vdir1/vdir2

   [A] on cherche 'vdir2' depuis '/vdir1/'

   1)	est-ce que vdir1 a un child du nom de vdir2?
   		oui -> on retourne ce child
		non -> existe pas, on retourne vide


	chemin complet : /vdir1/vdir2/vdir3

   [B] on cherche 'vdir2/vdir3' depuis '/vdir1/'

   1)	est-ce que vdir1 a un child du nom de vdir2?
   		oui -> on rappelle la fonction recursivement avec get_file_impl("vdir3", sp__/vdir1/vdir2 )
		2) est-ce que vdir2 a un child du nom de vdir3?
			oui -> on retourne ce child
			non -> existe pas, on retourne vide

		non -> existe pas, on retourne vide						 

 browse_folder (/vdir1/vdir2)
 calling get_file(/vdir1/vdir2, /)
 calling __get_file_impl(/ depuis /)
 found_id : -1
 found a folder to continue recursion : /
 calling __get_file_impl(vdir1 depuis /)
 found_id : -2
 found a folder to continue recursion : /vdir1
 calling __get_file_impl(vdir2 depuis /vdir1)
 found_id : -3
 found a folder to continue recursion : /vdir1/vdir2
 calling __get_file_impl( depuis /vdir1/vdir2)
 found_id : 0
 in handle_browse_folder /vdir1/vdir2 was not found


*/
		doc_file_item_pointer __get_file_impl(bfs::path::iterator start_itr, bfs::path::iterator end_itr, virtual_folder_pointer pvf)
		{
			LDBG_ << "calling __get_file_impl(" << *start_itr << " depuis " << pvf->get_path() << ")";

			doc_file_item_pointer pfound;
			int found_id;

			// treating root folder special case here
			if (*start_itr == "/" && pvf->get_id() == -1)
				found_id = -1;
			else
				// look for path element pointed to by current path iterator in pvf children
				found_id = pvf->for_each_child(find_child_with_name (*start_itr))._found_id;

			LDBG_ << "found_id : " << found_id;

			if (found_id != 0)
			{
				++start_itr;
				// if we just had the required path last element
				if (start_itr == end_itr)
				{
					LDBG_ << "end_itr has been got";

					// retrieve file with its id
					pfound = doc_database::get_instance().get_file(found_id);
				}
				// if its not yet the last path element, it has to be a folder to continue recursion
				else if (found_id < 0)
				{
					// it's a folder, cast it
					pfound = doc_database::get_instance().get_file(found_id);
					LDBG_ << "found a folder to continue recursion : " << pfound->get_path();
					virtual_folder_pointer pfoundvf = std::tr1::dynamic_pointer_cast<virtual_folder, doc_file_item>(pfound);
					if (pfoundvf)
						return __get_file_impl(start_itr, end_itr, pfoundvf);				
				}
			}
			return pfound;
		}

		doc_file_item_pointer doc_database::get_file(std::string path, virtual_folder_pointer pvf_from /*= get_root_vf()*/)
		{
			bfs::path dir_path(path); 
			
			LDBG_ << "calling get_file(" << path << ", " << pvf_from->get_path() << ")";
			doc_file_item_pointer pfi = __get_file_impl(dir_path.begin(), dir_path.end(), pvf_from);

			return pfi;
		}

		doc_file_item_pointer doc_database::get_file(int id)
		{
			doc_file_item_pointer pfile;
			file_map_t::iterator it = _files.find(id);
			if (it != _files.end())
				pfile = it->second;
			return pfile;
		}

	}

}
