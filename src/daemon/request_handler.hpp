#ifndef DOCLIB_DAEMON_REQUEST_HANDLER_HPP
#define DOCLIB_DAEMON_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace doclib
{

	namespace daemon
	{

		struct reply;
		struct request;

		/// The common handler for all incoming requests.
		class request_handler
			: private boost::noncopyable
		{
			public:
				/// 
				explicit request_handler();

				/// Handle a request and produce a reply.
				void handle_request(const request& req, reply& rep);
		};

	}

}

#endif // !DOCLIB_DAEMON_REQUEST_HANDLER_HPP
