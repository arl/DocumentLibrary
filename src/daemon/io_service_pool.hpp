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

#ifndef DOCLIB_DAEMON_IO_SERVICE_POOL_HPP
#define DOCLIB_DAEMON_IO_SERVICE_POOL_HPP

#include <boost/asio.hpp>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace doclib
{

	namespace daemon
	{

		/// A pool of io_service objects.
		class io_service_pool
			: private boost::noncopyable
		{
			public:
				/// Construct the io_service pool.
				explicit io_service_pool(std::size_t pool_size);

				/// Run all io_service objects in the pool.
				void run();

				/// Stop all io_service objects in the pool.
				void stop();

				/// Get an io_service to use.
				boost::asio::io_service& get_io_service();

			private:
				typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
				typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

				/// The pool of io_services.
				std::vector<io_service_ptr> io_services_;

				/// The work that keeps the io_services running.
				std::vector<work_ptr> work_;

				/// The next io_service to use for a connection.
				std::size_t next_io_service_;
		};

	}

}

#endif // !DOCLIB_DAEMON_IO_SERVICE_POOL_HPP
