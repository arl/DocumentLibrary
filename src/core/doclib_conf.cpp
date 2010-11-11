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

			if (bfs::exists(_filename)) {

				cout << "configuration file found : " << _filename << endl;

				// read configuration file
				inifile ini(_filename);
				if (ini.readFile()) {

					// load general config values
					long gal_id = ini.FindKey("general");
					if (gal_id != inifile::noID) {

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


					// load database config values
					long db_id = ini.FindKey("database");
					if (db_id != inifile::noID) {

						db_description_file = ini.GetValue(db_id, "db_description_file");
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

	}
}
