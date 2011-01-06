#include "doclib_conf.hpp"
#include "doclib_exception.hpp"
#include "inifile.hpp"

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;
using namespace std;

namespace doclib
{

    namespace core
	{

		/**
		 * create default configuration
		 */
		doclib_conf::doclib_conf()
		{
			// log_level set to minimum log output (only informationnal)
			log_level = bl::level::info;
		}

		/**
		 * load configuration from given filename
		 */
		bool doclib_conf::load(string _filename)
		{
			string line;
			bool ret = false;

			if (bfs::exists(_filename))
			{
				cout << "configuration file found : " << _filename << endl;

				// read configuration file
				inifile ini(_filename);
				if (ini.readFile())
				{

/**
* @todo treat cases where key exists but values dont
**/


					// load general config values
					long gal_id = ini.FindKey("general");
					if (gal_id != inifile::noID)
					{
						log_filename = ini.GetValue(gal_id, "log_file");
						string val = ini.GetValue(gal_id, "log_level");
						if (val == "debug")
							log_level = bl::level::debug;
						if (val == "error")
							log_level = bl::level::error;

						// if successfull, set conf filename
						filename = filename;
					}
					else
					{
						// use default values for general key
						log_filename = "/var/log/doclibd.log";			
						log_level = bl::level::debug;
					}


					// load daemon config values
					long daemon_id = ini.FindKey("daemon");
					if (daemon_id != inifile::noID)
					{
						port = ini.GetValueI(daemon_id, "port", (unsigned int)-1);
						if (port == (unsigned int) -1)
						{
							string msg ("no daemon port number found in configuration in \"");
							msg.append(_filename);
							msg.append("\"");
							throw doclib_exception(msg);
						}
						num_threads = ini.GetValueI(daemon_id, "num_threads", (unsigned int)-1);
						{
							string msg ("no daemon number of threads found in configuration in \"");
							msg.append(_filename);
							msg.append("\"");
							throw doclib_exception(msg);
						}
					}
					else
					{
						// no default values for daemon keys
						string msg ("no database description file found in configuration in \"");
						msg.append(_filename);
						msg.append("\"");
						throw doclib_exception(msg);
					}


					// load database config values
					long db_id = ini.FindKey("database");
					if (db_id != inifile::noID)
					{
						db_description_file = ini.GetValue(db_id, "db_description_file", "//\\");
						ret = true;
					}
					else
					{
						// no default values for database key
						string msg ("no database description file found in configuration in \"");
						msg.append(_filename);
						msg.append("\"");
						throw doclib_exception(msg);
					}

				}
			}
			// if successfull, set conf filename
			if (ret)
				filename = filename;
			return ret;
		}

		/**
		 * save current configuration to given filename
		 */
		bool doclib_conf::save(string filename)
		{
			return true;
		}

		std::string doclib_conf::get_filename() const
		{
			return filename;
		}

		std::string doclib_conf::get_log_filename() const
		{
			return log_filename;
		}

		std::string doclib_conf::get_db_description_file() const
		{
			return db_description_file;
		}

		bl::level::type	doclib_conf::get_log_level() const
		{
			return log_level;
		}

		unsigned int doclib_conf::get_port() const
		{
			return port;
		}

		unsigned int doclib_conf::get_num_threads() const
		{
			return num_threads;
		}

	}
}
