#ifndef DOCLIB_CORE_DOCLIB_CONF_HPP
#define DOCLIB_CORE_DOCLIB_CONF_HPP

#include "core_includes.hpp"

namespace doclib
{

	namespace core
	{

		using namespace std;


		class doclib_conf
		{
			public:

				/**
				 * create default configuration 
				 */
				doclib_conf();

				/**
				 * load configuration from given filename 
				 */
				bool load(string filename);

				/**
				 * save current configuration to given filename
				 */
				bool save(string filename);

				/**
				* @brief get config filename
				*
				* @return filename 
				*/
				std::string get_filename() const;

				/**
				* @brief get log file path
				*
				* @return log file path
				*/
				std::string get_log_filename() const;

				/**
				* @brief get database description file name
				*
				* @return db description file name
				*/
				std::string get_db_description_file() const;

				/**
				* @brief get log level
				*
				* @return log level
				*/
				bl::level::type	get_log_level() const;

				/**
				* @brief get daemon server port number
				*
				* @return port number
				*/
				unsigned int get_port() const;


				/**
				* @brief get number of threads for daemon
				*
				* @return number of threads
				*/
				unsigned int get_num_threads() const;


			private:

				/// configuration filename
				string	filename;

				/// log file path
				string	log_filename;

				/// logger level
				bl::level::type	log_level;

				/// server listening port
				unsigned int port;

				/// daemon number of threads
				unsigned int num_threads;

				/// document database description file path
				string	db_description_file;
		};

	}

}

#endif // !DOCLIB_CORE_DOCLIB_CONF_HPP
