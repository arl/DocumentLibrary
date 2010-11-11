#include "doclib_core.hpp" 
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/logging/format/named_write.hpp>

// logger definition
BOOST_DEFINE_LOG(g_l, log_type)
BOOST_DEFINE_LOG_FILTER(g_l_filter, bl::level::holder)

namespace doclib
{
	namespace core
	{
/*
		bool init_daemon_or_quit()
		{
			try
			{
				//This throws if the file does not exist or it can't
				//open it with read-write access!
				boost::interprocess::file_lock flock(DAEMON_MUTEX_FILENAME);

				init_logging();

				return true;
			}

			catch(boost::interprocess::interprocess_exception &e)
			{
				//std::cout << "another instance of doclib is running or previous instanciation had not ended correctly" << std::endl;

				return false;
			}
		}
*/

		// static instanciation
		core_services* core_services::_inst = 0;

		// private constructor
		core_services::core_services()
		{
			// nothing to do
		}


		void core_services::create()
		{
			if (_inst != 0)
			{
				std::cerr << "internal error : core_services already exists" << std::endl;
				throw std::exception();
			}
			_inst = new core_services;
		}

		bool core_services::exists()
		{
			return _inst != 0;
		}

		core_services& core_services::get()
		{
			if (_inst == 0)
			{
				std::cerr << "internal error : core_services doesn't exist" << std::endl;
				throw std::exception();
			}
			return *_inst;
		}

		void core_services::init_logging(string log_file, bl::level::type log_level)
		{
			// define filter on chosen level
			g_l_filter()->set_enabled(log_level);

			// formatting    : [idx] time [level] message \n
			// destinations  : console, log_file and debug window
			g_l()->writer().add_formatter( bl::formatter::idx());
			g_l()->writer().add_formatter( bl::formatter::time("$hh:$mm.$ss "));
			g_l()->writer().add_formatter( bl::formatter::append_newline());

			//        ... and where should it be written to
			g_l()->writer().add_destination( bl::destination::cout() );
			g_l()->writer().add_destination( bl::destination::dbg_window() );
			g_l()->writer().add_destination( bl::destination::file(log_file) );
			g_l()->turn_cache_off();
		}

		
	}

}
