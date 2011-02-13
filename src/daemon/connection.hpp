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

#ifndef DOCLIB_DAEMON_CONNECTION_HPP
#define DOCLIB_DAEMON_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace doclib
{

	namespace daemon
	{

		/// Represents a single connection from a client.
		class connection
			: public boost::enable_shared_from_this<connection>,
			private boost::noncopyable
		{
			public:
				/// Construct a connection with the given io_service.
				explicit connection(boost::asio::io_service& io_service,
						request_handler& handler);

				/// Get the socket associated with the connection.
				boost::asio::ip::tcp::socket& socket();

				/// Start the first asynchronous operation for the connection.
				void start();

			private:
				/// Handle completion of a read operation.
				void handle_read(const boost::system::error_code& e,
						std::size_t bytes_transferred);

				/// Handle completion of a write operation.
				void handle_write(const boost::system::error_code& e);

				/// Socket for the connection.
				boost::asio::ip::tcp::socket socket_;

				/// The handler used to process the incoming request.
				request_handler& request_handler_;

				/// Buffer for incoming data.
				boost::array<char, 8192> buffer_;

				/// The incoming request.
				request request_;

				/// The parser for the incoming request.
				request_parser request_parser_;

				/// The reply to be sent back to the client.
				reply reply_;
		};

		typedef boost::shared_ptr<connection> connection_ptr;

	}

}

#endif // !DOCLIB_DAEMON_CONNECTION_HPP
