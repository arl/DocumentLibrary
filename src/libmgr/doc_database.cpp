#include "doc_database.hpp"
#include "virtual_folder.hpp"

namespace doclib
{

    namespace libmgr
    {

		/**
		* @brief virtual folder visitor for xml vi_folder description file
		*/
		class doc_database::v_folder_visitor : public TiXmlVisitor
		{
			public:
				ostream & out;
				virtual_folder * _proot_vf;
				virtual_folder * _pcur_vf;

				v_folder_visitor(virtual_folder *& proot_vf) : _proot_vf(proot_vf), _pcur_vf(0), out(cout) {}
				virtual ~v_folder_visitor() {}

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
							// create root virtual folder
							_proot_vf = new virtual_folder(name, -1, (virtual_folder*)0);
							// let it be the current vf
							_pcur_vf = _proot_vf;
						}
						else
						{
							// create a new virtual folder
							virtual_folder * pnew_vf = new virtual_folder(name, id, _pcur_vf);
							// add it to current vf
							_pcur_vf->add_child(pnew_vf);
							// let the newly created virtual folder be the current one
							_pcur_vf = pnew_vf;
						}
					}
					else if (tag_name == "children")
					{
					
					}
					else if (tag_name == "file")
					{
					
					}
					else if (tag_name == "path")
					{
					
					}

					return true;
				}

				/// Visit an element.
				virtual bool VisitExit( const TiXmlElement& elt )
				{
					out << "element::VisitExit " << elt.Value() << endl;
					return true;
				}

				/// Visit a text node
				virtual bool Visit( const TiXmlText& text )	
				{
					out << "text::Visit " << text.Value() << endl;
					return true;
				}
		};

		/**
		* @brief virtual folder visitor for xml vi_folder description file
		*/
		class v_folder_visitor_test : public TiXmlVisitor
		{
			public:
				ostream & out;

				v_folder_visitor_test(ostream & _out) : out(_out) {}

				virtual ~v_folder_visitor_test() {}

				/// Visit an element.
				virtual bool VisitEnter( const TiXmlElement& elt, const TiXmlAttribute* /*first_attr*/)
				{
					out << "element::VisitEnter " << elt.Value() << endl;
					return true;
				}

				/// Visit an element.
				virtual bool VisitExit( const TiXmlElement& elt )
				{
					out << "element::VisitExit " << elt.Value() << endl;
					return true;
				}

				/// Visit a text node
				virtual bool Visit( const TiXmlText& text )	
				{
					out << "text::Visit " << text.Value() << endl;
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

		doc_database& doc_database::get_instance()
		{
			assert(p_instance);
			return *p_instance;
		}

        doc_database::doc_database(const core::doclib_conf & conf)
            :	_conf(conf)
			,	_proot_vf (0)
        {
        }

		bool doc_database::load_database()
		{
			if (load_description_file())
			{
				// TODO CONTINUE HEER
				// TODO CONTINUE HEER
				// TODO CONTINUE HEER
				// TODO CONTINUE HEER
				return true;
			}
			LERR_ << "document database description file not found \"" << _conf.db_description_file << "\"";
			return false;
		}

		struct virtual_folder_content
		{
			string name;
			int id;
		};

		void fill_virtual_folder_content(const TiXmlElement * pelt, virtual_folder_content & content)
		{
		
				
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
				
							v_folder_visitor * visitor = new v_folder_visitor(_proot_vf);
							pvirt_fold->Accept(visitor);


							//  TODO : fill read virtual_folders with the help of a TixmlVisitor
							//  TODO : fill read virtual_folders with the help of a TixmlVisitor
							//  TODO : fill read virtual_folders with the help of a TixmlVisitor
							//  TODO : fill read virtual_folders with the help of a TixmlVisitor
/*
							TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();

							for( child; child; child=child->NextSiblingElement() )
							{
								// do something
							}*/

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
