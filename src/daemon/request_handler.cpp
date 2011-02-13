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

#include "../core/doclib_core.hpp"
#include "../libmgr/doc_database.hpp"
#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "reply.hpp"
#include "request.hpp"

namespace doclib
{

	namespace daemon
	{


		request_handler::request_handler()
		{
		}

		void request_handler::handle_request(const request& req, reply& rep)
		{
			// DEBUG : to test server simultaneous request responding
			std::string req_str (req.to_string());
			LDBG_ << req_str;

			switch (req.action)
			{
			case request::view_document:
				break;
			case request::download_document:
				break;
			case request::search_query:
				break;
			case request::browse_folder:
				handle_browse_folder(req, rep);
				break;
			}

			return;
		}

		void request_handler::handle_browse_folder(const request& req, reply& rep)
		{
			if (req.args.size() < 1)
			{
				rep.stock_reply(reply::bad_arg_number);
				return;
			}
			// return folder content
			using namespace doclib::libmgr;
			doc_database& db = doc_database::get_instance();
			doc_file_item_pointer pvf = db.get_file(req.args[0]);

			bfs::path fs_path(req.args[0]);	
			bfs::path::iterator itr = fs_path.begin();
			bfs::path::iterator end_itr = fs_path.begin();
			std::size_t count = 0;
			while (itr != end_itr)
			{
				LDBG_ << "counting iterators (" << ++count << ") -> " << *itr;
				++itr;
			}


			if (pvf)
				LDBG_ << "in handle_browse_folder " << req.args[0] << " was found, pvf->path() : " << pvf->get_path();
			else
				LDBG_ << "in handle_browse_folder " << req.args[0] << " was not found";
		}
	}

}
