#include "doc_database.hpp"
#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

		/**
		* @brief information printing predicate for file item
		* @note	useful for debugging purposes
		*/
		struct print_file_item
		{
			print_file_item() : indent(0) {}

			int indent;
	
			void operator () (sp_doc_file_item spfi)
			{
				//if (pfi->get_file_type() == "vfolder")
				//{
					//std::cout << "virtual folder (" << pfi->get_path() << ", " << pfi->get_id() << ")" << std::endl;
					L_ << spfi->get_filetype()  << " : (" << spfi->get_path() << ", " << spfi->get_id() << ")" ;
				//}

			}
		};

		/**
		* @brief predicate for deallocating file items
		*/
		/*
		struct free_file_item
		{
			void operator () (doc_file_item * pfi)
			{
				LDBG_ << "freeing " << pfi->get_path() << " at address " << lexical_cast<string>(pfi);
				assert(pfi);
				delete pfi;
				pfi = 0;
			}
		};
		*/


		/**
		* @brief virtual folder visitor for xml vi_folder description file
		*/
		class doc_database::v_folder_visitor : public TiXmlVisitor
		{
			private:

				ostream & out;
				sp_virtual_folder _sproot_vf;
				sp_virtual_folder _spcur_vf;

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


				sp_virtual_folder get_root_vf() { return _sproot_vf; }
				

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
							sp_virtual_folder sp_empty;	

							// let create the root virtual folder as a shared pointer
							_spcur_vf = _sproot_vf = sp_virtual_folder (new virtual_folder(name, -1, sp_empty));
							LDBG_ << "allocating root at address " << lexical_cast<string>(_sproot_vf.get());
						}
						else
						{
							// create a new virtual folder
							sp_virtual_folder sp_new_vf = sp_virtual_folder (new virtual_folder(name, id, _spcur_vf));

							LDBG_ << "allocating " << sp_new_vf->get_path()  << " at address " << lexical_cast<string>(sp_new_vf.get());
							// add it to current vf
							_spcur_vf->add_child(sp_new_vf);
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
							// let current virtual folder parent be the new current one
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
						sp_doc_file_item spf = sp_doc_file_item(new doc_file_item(_last_id, file.string(), file.extension(), _spcur_vf));
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
			LERR_ << "document database description file not found \"" << _conf.db_description_file << "\"";
			return false;
		}

		bool doc_database::load_description_file()
		{
			bool ret = false;

			TiXmlDocument doc;
			if (bfs::exists(_conf.db_description_file))
			{
				if (doc.LoadFile(_conf.db_description_file))
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
							LERR_ << "syntax error in document database description \"" << _conf.db_description_file << "\"" << " no <root_path> and/or root <virtual folder> tag found";

					}
					else
						LERR_ << "syntax error in document database description \"" << _conf.db_description_file << "\"" << " no <folder_structure> tag found";
				}
				else
					LERR_ << "can not load document database description \"" << _conf.db_description_file << "\"" << doc.ErrorDesc();
			}

			return ret;
		}

	}

}
