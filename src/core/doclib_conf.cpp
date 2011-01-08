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

		doclib_conf::doclib_conf()
		{
			// log_level set to minimum log output (only informationnal)
			log_level = bl::level::info;
		}

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

					// load general config values
					long gal_id = ini.FindKey("general");
					if (gal_id != inifile::noID)
					{
						long log_file_id = ini.FindValue(gal_id, "log_file");
						long log_level_id = ini.FindValue(gal_id, "log_level");

						if (log_file_id != inifile::noID)
							log_filename = ini.GetValue(gal_id, log_file_id);
						else
						{
							// use default log file value
							log_filename = "/var/log/doclibd.log";			
							cout << "could not find general::log_file value in \"" << _filename << "\", defaulting to \'" << log_filename << "\'" << std::endl;
						}

						string val;
						if (log_level_id != inifile::noID)
						{
							val = ini.GetValue(gal_id, log_level_id);
							if (val == "debug")
								log_level = bl::level::debug;
							else if (val == "info")
								log_level = bl::level::info;
							else if (val == "error")
								log_level = bl::level::error;
							else
								// use default log file value
								cout << "bad value for general::log_level in \"" << _filename << "\", defaulting to \'info\'" << std::endl;
						}
						else
							// use default log file value
							cout << "could not find general::log_level value in \"" << _filename << "\", defaulting to \'info\'" << std::endl;

						/// @todo : put that at the end of function, when everything is sure ok
						// if successfull, set conf filename
						filename = _filename;
					}
					else
					{
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
						if (num_threads == (unsigned int) -1)
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
