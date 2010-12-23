#ifndef DOCLIB_DAEMON_REQUEST_HPP
#define DOCLIB_DAEMON_REQUEST_HPP

#include <string>
#include <vector>
#include "reply.hpp"

namespace doclib
{

	namespace daemon
	{

		/// A request received from a client.
		struct request
		{
			std::string method;
			std::string uri;
			int http_version_major;
			int http_version_minor;
			std::vector<header> headers;
		};

	}

}

#endif // !DOCLIB_DAEMON_REQUEST_HPP
