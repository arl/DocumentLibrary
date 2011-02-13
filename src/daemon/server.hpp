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

#ifndef DOCLIB_DAEMON_SERVER_HPP
#define DOCLIB_DAEMON_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "io_service_pool.hpp"
#include "request_handler.hpp"

namespace doclib
{

	namespace daemon
	{

		/// The top-level class of the HTTP server.
		class server
			: private boost::noncopyable
		{
		public:
		  /// Construct the server to listen on the specified TCP address and port, and
		  /// serve up files from the given directory.
		  explicit server(const std::string& address, const std::string& port,
			  std::size_t io_service_pool_size);

		  /// Run the server's io_service loop.
		  void run();

		  /// Stop the server.
		  void stop();

		private:
		  /// Handle completion of an asynchronous accept operation.
		  void handle_accept(const boost::system::error_code& e);

		  /// The pool of io_service objects used to perform asynchronous operations.
		  io_service_pool io_service_pool_;

		  /// Acceptor used to listen for incoming connections.
		  boost::asio::ip::tcp::acceptor acceptor_;

		  /// The next connection to be accepted.
		  connection_ptr new_connection_;

		  /// The handler for all incoming requests.
		  request_handler request_handler_;
		};

	}

}

#endif // !DOCLIB_DAEMON_SERVER_HPP
