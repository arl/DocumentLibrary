#include "server.hpp"
#include <boost/bind.hpp>

namespace doclib
{

	namespace daemon
	{

		server::server(const std::string& address, const std::string& port,
			std::size_t io_service_pool_size)
		  : io_service_pool_(io_service_pool_size),
			acceptor_(io_service_pool_.get_io_service()),
			new_connection_(new connection(
				  io_service_pool_.get_io_service(), request_handler_))
		{
		  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		  boost::asio::ip::tcp::resolver resolver(acceptor_.io_service());
		  boost::asio::ip::tcp::resolver::query query(address, port);
		  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		  acceptor_.open(endpoint.protocol());
		  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		  acceptor_.bind(endpoint);
		  acceptor_.listen();
		  acceptor_.async_accept(new_connection_->socket(),
			  boost::bind(&server::handle_accept, this,
				boost::asio::placeholders::error));
		}

		void server::run()
		{
		  io_service_pool_.run();
		}

		void server::stop()
		{
		  io_service_pool_.stop();
		}

		void server::handle_accept(const boost::system::error_code& e)
		{
		  if (!e)
		  {
			new_connection_->start();
			new_connection_.reset(new connection(
				  io_service_pool_.get_io_service(), request_handler_));
			acceptor_.async_accept(new_connection_->socket(),
				boost::bind(&server::handle_accept, this,
				  boost::asio::placeholders::error));
		  }
		}

	}

}

