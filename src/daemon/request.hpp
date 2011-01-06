#ifndef DOCLIB_DAEMON_REQUEST_HPP
#define DOCLIB_DAEMON_REQUEST_HPP

#include <string>
#include <vector>
#include "reply.hpp"

namespace doclib
{

	namespace daemon
	{
//
//		/// A request received from a client.
//		struct request
//		{
//			std::string method;
//			std::string uri;
//			int http_version_major;
//			int http_version_minor;
//			std::vector<header> headers;
//		};
//
//

        /** request protocol description


request packet global structure

     action  |  num_args  | [argument(s)  packet-end
    [   2  ][1][   U    ][1][           ][     1    ]


    arguments field structure
         arg_size  |     arg 1         |  arg_size  |      arg 2
        [   U    ][1][   arg_size    ][1][   U    ][1][   arg_size    ]


U : undefined number of characters (content is what is between 2 field separators)

action : 2 ascii chars describing request action

    VI : view_document
    DW : download_document
    QY : search_query


field separator : '|' character



        */

        /// reply protocol description



		/**
		* @brief Represents a request received from a client
		*/
        struct request
        {
			/**
			* @brief request action code
			*/
            enum action
            {
                view_document,
                download_document,
                search_query
            } action_;

			/**
			* @brief request action
			*/
            int action;

			/**
			* @brief request arguments
			*/
            std::vector<std::string> args;

			/**
			* @brief request argument sizes 
			*/
            std::vector<std::size_t> arg_sizes;

			/**
			* @brief request constructor
			*/
			request() : action(-1) {}

			/**
			* @brief request copy constructor
			*
			* @param copy instance to copy
			*/
			request(const request& copy)
				: action(copy.action)
			{
				std::copy(copy.arg_sizes.begin(), copy.arg_sizes.end(),
						std::back_inserter(arg_sizes));
				std::copy(copy.args.begin(), copy.args.end(),
						std::back_inserter(args));
			}
		};


	}

}

#endif // !DOCLIB_DAEMON_REQUEST_HPP
