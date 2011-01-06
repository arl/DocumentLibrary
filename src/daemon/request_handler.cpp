#include "../core/doclib_core.hpp"
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
			// Decode url to path.
			std::string request_path;

			// DEBUG : to test server simultaneous request responding
            LDBG_ << "request received : " << req.action;
			rep.content = "request treated : " + boost::lexical_cast<std::string>(req.action);
			return;
		}
	}

}
