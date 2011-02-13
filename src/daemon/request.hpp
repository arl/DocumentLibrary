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
	BW : browse_folder


field separator : '|' character



        */

        /// reply protocol description



		/**
		* @brief Request received from a client
		*/
        struct request
        {
			/**
			* @brief request action code
			*/
			enum e_action
			{
				view_document,
				download_document,
				search_query,
				browse_folder
			};

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
			 * @brief convert a request to a human readable function-like string
			 *
			 * @param req the request to convert
			 *
			 * @note for debugging purposes
			 *
			 * @return a function like string
			 */
			std::string to_string() const
			{
				std::string str;

				switch (action)
				{
				case view_document:
					str.assign("view_document (");
					break;
				case download_document:
					str.assign("download_document (");
					break;
				case search_query:
					str.assign("search_query (");
					break;
				case browse_folder:
					str.assign("browse_folder (");
					for (std::size_t i = 0; i < args.size(); ++i)
					{
						switch (i)
						{
						case 0:
							str.append(args[0]);
							break;
						}
					}
					str.append(")");

					break;
				}
				return str;
			}

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
