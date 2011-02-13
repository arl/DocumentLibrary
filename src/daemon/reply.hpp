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

#ifndef DOCLIB_DAEMON_REPLY_HPP
#define DOCLIB_DAEMON_REPLY_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace doclib
{

	namespace daemon
	{

		/// A reply to be sent to a client.
		struct reply
		{
			/// The status of the reply.
			enum status_type
			{
				ok = 200,						/// ok, request succeeded

				// client-side errors
				bad_request = 400,				/// badly formed request (syntactically speaking)

				// user-side errors
				not_found = 404,				/// requested folder/file not found

				// server-side errors
				not_implemented = 501,			/// functionality not implemented (yet...)
				internal_server_error = 502,	/// internal server error
				bad_arg_number = 503			/// bad number of arguments in request


				/*
				created = 201,
				accepted = 202,
				no_content = 204,
				multiple_choices = 300,
				moved_permanently = 301,
				moved_temporarily = 302,
				not_modified = 304,
				bad_request = 400,
				unauthorized = 401,
				forbidden = 403,
				not_found = 404,
				internal_server_error = 500,
				not_implemented = 501,
				bad_gateway = 502,
				service_unavailable = 503
				*/
			} status;

			/**
			* @brief reply constructor
			*/
			reply() : status(ok) {}


			/**
			* @brief reply arguments
			*/
			std::vector<std::string> args;

			/**
			* @brief reply arguments respective size
			*/
			std::vector<std::size_t> arg_sizes;

			/**
			* @brief status code string
			*/
			std::string status_str;

			/**
			* @brief arguments count string
			*/
			std::string args_count_str;

			/**
			* @brief reply arguments respective size
			*/
			std::vector<std::string> arg_sizes_str;

			/**
			* @brief Convert the reply into a vector of buffers.
			*	The buffers do not own the underlying memory blocks, therefore
			*	the reply object must remain valid and not be changed until the
			*	write operation has completed.
			*/
			std::vector<boost::asio::const_buffer> to_buffers();

			/// Create a stock reply.
			void stock_reply(status_type status_);
		};

		/// convert a reply status to a human readable string
		std::string to_string(reply::status_type status);

	}

}

#endif // !DOCLIB_DAEMON_REPLY_HPP
