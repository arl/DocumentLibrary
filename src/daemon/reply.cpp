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

#include "reply.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

namespace doclib
{

	namespace daemon
	{

		namespace status_strings
		{
/*
							const std::string created =
								"HTTP/1.0 201 Created\r\n";
							const std::string accepted =
								"HTTP/1.0 202 Accepted\r\n";
							const std::string no_content =
								"HTTP/1.0 204 No Content\r\n";
							const std::string multiple_choices =
								"HTTP/1.0 300 Multiple Choices\r\n";
							const std::string moved_permanently =
								"HTTP/1.0 301 Moved Permanently\r\n";
							const std::string moved_temporarily =
								"HTTP/1.0 302 Moved Temporarily\r\n";
							const std::string not_modified =
								"HTTP/1.0 304 Not Modified\r\n";
							const std::string bad_request =
								"HTTP/1.0 400 Bad Request\r\n";
							const std::string unauthorized =
								"HTTP/1.0 401 Unauthorized\r\n";
							const std::string forbidden =
								"HTTP/1.0 403 Forbidden\r\n";
							const std::string not_found =
								"HTTP/1.0 404 Not Found\r\n";
							const std::string internal_server_error =
								"HTTP/1.0 500 Internal Server Error\r\n";
							const std::string not_implemented =
								"HTTP/1.0 501 Not Implemented\r\n";
							const std::string bad_gateway =
								"HTTP/1.0 502 Bad Gateway\r\n";
							const std::string service_unavailable =
								"HTTP/1.0 503 Service Unavailable\r\n";*/
				
/*
			
						boost::asio::const_buffer to_buffer(reply::status_type status)
						{
							switch (status)
							{
								case reply::ok:
									return boost::asio::buffer(ok);
								case reply::created:
									return boost::asio::buffer(created);
								case reply::accepted:
									return boost::asio::buffer(accepted);
								case reply::no_content:
									return boost::asio::buffer(no_content);
								case reply::multiple_choices:
									return boost::asio::buffer(multiple_choices);
								case reply::moved_permanently:
									return boost::asio::buffer(moved_permanently);
								case reply::moved_temporarily:
									return boost::asio::buffer(moved_temporarily);
								case reply::not_modified:
									return boost::asio::buffer(not_modified);
								case reply::bad_request:
									return boost::asio::buffer(bad_request);
								case reply::unauthorized:
									return boost::asio::buffer(unauthorized);
								case reply::forbidden:
									return boost::asio::buffer(forbidden);
								case reply::not_found:
									return boost::asio::buffer(not_found);
								case reply::internal_server_error:
									return boost::asio::buffer(internal_server_error);
								case reply::not_implemented:
									return boost::asio::buffer(not_implemented);
								case reply::bad_gateway:
									return boost::asio::buffer(bad_gateway);
								case reply::service_unavailable:
									return boost::asio::buffer(service_unavailable);
								default:
									return boost::asio::buffer(internal_server_error);
							}
						}*/
			

		} // namespace status_strings

		namespace misc_strings {

			const char separator[] = { '|' };
			const char crlf[] = { '\r', '\n' };

		} // namespace misc_strings

		std::vector<boost::asio::const_buffer> reply::to_buffers()
		{
			// write status code string
			std::vector<boost::asio::const_buffer> buffers;

			status_str = boost::lexical_cast<std::string>(status);
			buffers.push_back(boost::asio::buffer(status_str));
			buffers.push_back(boost::asio::buffer(misc_strings::separator));
			
			// write number of arguments
			args_count_str = boost::lexical_cast<std::string>(args.size());
			buffers.push_back(boost::asio::buffer(args_count_str));
			buffers.push_back(boost::asio::buffer(misc_strings::separator));
			
			// write all arguments
			arg_sizes_str.clear();
			for (std::size_t i = 0; i < args.size(); ++i)
			{
				arg_sizes_str.push_back(boost::lexical_cast<std::string>(arg_sizes[i]));
				// write size of current argument
				buffers.push_back(boost::asio::buffer(arg_sizes_str[i]));
				buffers.push_back(boost::asio::buffer(misc_strings::separator));
			
				// write argument content
				buffers.push_back(boost::asio::buffer(args[i]));
				buffers.push_back(boost::asio::buffer(misc_strings::separator));
			}
						
			// write and separator
			buffers.push_back(boost::asio::buffer(misc_strings::separator));
			
			return buffers;
		}



		namespace stock_replies {
		} // namespace stock_replies

		const char ok[] =
			"";
		const char bad_request[] =
			"Request syntax error";
		const char not_found[] =
			"Request syntax error";
		const char not_implemented[] =
			"Not Implemented";
		const char internal_server_error[] =
			"Internal Server Error";
		const char bad_arg_number[] =
			"Bad number of arguments";

		/*
		   const char created[] =
		   "<html>"
		   "<head><title>Created</title></head>"
		   "<body><h1>201 Created</h1></body>"
		   "</html>";
		   const char accepted[] =
		   "<html>"
		   "<head><title>Accepted</title></head>"
		   "<body><h1>202 Accepted</h1></body>"
		   "</html>";
		   const char no_content[] =
		   "<html>"
		   "<head><title>No Content</title></head>"
		   "<body><h1>204 Content</h1></body>"
		   "</html>";
		   const char multiple_choices[] =
		   "<html>"
		   "<head><title>Multiple Choices</title></head>"
		   "<body><h1>300 Multiple Choices</h1></body>"
		   "</html>";
		   const char moved_permanently[] =
		   "<html>"
		   "<head><title>Moved Permanently</title></head>"
		   "<body><h1>301 Moved Permanently</h1></body>"
		   "</html>";
		   const char moved_temporarily[] =
		   "<html>"
		   "<head><title>Moved Temporarily</title></head>"
		   "<body><h1>302 Moved Temporarily</h1></body>"
		   "</html>";
		   const char not_modified[] =
		   "<html>"
		   "<head><title>Not Modified</title></head>"
		   "<body><h1>304 Not Modified</h1></body>"
		   "</html>";
		   const char unauthorized[] =
		   "<html>"
		   "<head><title>Unauthorized</title></head>"
		   "<body><h1>401 Unauthorized</h1></body>"
		   "</html>";
		   const char forbidden[] =
		   "<html>"
		   "<head><title>Forbidden</title></head>"
		   "<body><h1>403 Forbidden</h1></body>"
		   "</html>";
		   const char not_found[] =
		   "<html>"
		   "<head><title>Not Found</title></head>"
		   "<body><h1>404 Not Found</h1></body>"
		   "</html>";
		   const char bad_gateway[] =
		   "<html>"
		   "<head><title>Bad Gateway</title></head>"
		   "<body><h1>502 Bad Gateway</h1></body>"
		   "</html>";
		   const char service_unavailable[] =
		   "<html>"
		   "<head><title>Service Unavailable</title></head>"
		   "<body><h1>503 Service Unavailable</h1></body>"
		   "</html>";
		   */


		std::string to_string(reply::status_type status)
		{
			switch (status)
			{
				case reply::ok:
					return ok;
				case reply::bad_request:
					return bad_request;
				case reply::not_found:
					return not_found;
				case reply::not_implemented:
					return not_implemented;
				case reply::bad_arg_number:
					return bad_arg_number;
				case reply::internal_server_error:
				default:
					return internal_server_error;

					/*
					   case reply::created:
					   return created;
					   case reply::accepted:
					   return accepted;
					   case reply::no_content:
					   return no_content;
					   case reply::multiple_choices:
					   return multiple_choices;
					   case reply::moved_permanently:
					   return moved_permanently;
					   case reply::moved_temporarily:
					   return moved_temporarily;
					   case reply::not_modified:
					   return not_modified;
					   case reply::bad_request:
					   return bad_request;
					   case reply::unauthorized:
					   return unauthorized;
					   case reply::forbidden:
					   return forbidden;
					   case reply::not_found:
					   return not_found;
					   case reply::internal_server_error:
					   return internal_server_error;
					   case reply::not_implemented:
					   return not_implemented;
					   case reply::bad_gateway:
					   return bad_gateway;
					   case reply::service_unavailable:
					   return service_unavailable;*/

			}
		}

		void reply::stock_reply(reply::status_type status_)
		{
			status = status_;
			args.clear();

			args.push_back(to_string(status));
			arg_sizes.push_back(to_string(status).size());
		}

	}
}
