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
				 * @brief constructor.
				 *
				 * create default configuration.
				 */
				doclib_conf();

				/**
				 * @brief load configuration from given filename.
				 *
				 * If loading succeeds, then get_filename returns the filename passed as argument.
				 * @sa get_filename
				 *
				 * @param _filename file to load.
				 *
				 * @return true on success (success means that all mandatory values have been found.
				 *
				 * @todo finish treating cases where key exists but values dont
				 */
				bool load(string filename);

				/**
				 * @brief save current configuration to given filename.
				 */
				bool save(string filename);

				/**
				 * @brief returns current configuration file name.
				 *
				 * If configuration loading failed, then get_filename should not be used.
				 *
				 * @return configuration file name
				 */
				std::string get_filename() const;

				/**
				 * @brief returns log file name.
				 *
				 * @return log file name
				 */
				std::string get_log_filename() const;

				/**
				 * @brief returns database description file name.
				 *
				 * @return database description file name
				 */
				std::string get_db_description_file() const;

				/**
				 * @brief returns current log level.
				 *
				 * @return log level
				 */
				bl::level::type	get_log_level() const;

				/**
				 * @brief returns daemon port number
				 *
				 * @return port number
				 */
				unsigned int get_port() const;

				/**
				 * @brief returns daemon number of threads
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
