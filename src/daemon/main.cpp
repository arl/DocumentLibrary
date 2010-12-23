#include "doclibd_includes.hpp"
#include "../core/doclib_core.hpp"
#include "../libmgr/doc_database.hpp"
#include "../version.hpp"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.hpp"

#if !defined(_WIN32)
# include <pthread.h>
# include <signal.h>
#endif // !_WIN32

using namespace std;

/**
 * Enumerations of the different possibilities after application startup
 */
enum eStartupReturn
{
	esr_PRINT_HELP
};

// local instances
string config_file;
string version_str;
doclib::core::doclib_conf cfg;

void set_version_string()
{
	version_str.assign(lexical_cast<string>(doclib::version::major));
	version_str.append(".");
	version_str.append(lexical_cast<string>(doclib::version::minor));
	version_str.append(".");
	version_str.append(lexical_cast<string>(doclib::version::build));
}

int treat_command_line(int argc, char ** argv)
{
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("config-file,c", po::value<string>(), "set configuration from filename instead")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << "DocLibrary version " << version_str << "\n";
		cout << desc << "\n";
		return EXIT_FAILURE;
	}

	if (vm.count("config-file"))
	{
		cout << "using "
			<< vm["config-file"].as<string>() << " as configuration file." << endl;
		config_file = vm["config-file"].as<string>();
	} else {
		// set default configuration file
		config_file = string(DOCLIBD_DEFAULT_CFG_FILENAME);
	}

	return EXIT_SUCCESS;
}



int main(int argc, char ** argv)
{
	int ret = EXIT_FAILURE;

	try
	{
		set_version_string();
		cout << "DocumentLibrary version " << version_str << endl;
		treat_command_line(argc, argv);

		if (cfg.load(config_file))
		{
			// init log instance, providing it 'ini file loaded' values
			doclib::core::core_services::init_logging(cfg.log_filename, cfg.log_level);
			LDBG_ << "successfully loaded configuration file";

			doclib::libmgr::doc_database::create_instance(cfg);

			if (doclib::libmgr::doc_database::get_instance().load_database())
			{
				LDBG_ << "successfully loaded document database";
				ret = EXIT_SUCCESS;


				// server default parameters
				string address = "0.0.0.0";
				string port = "36000";
				string doc_root = "/";
				std::size_t num_threads = 2;

// posix server
#if !defined(_WIN32)

				// Block all signals for background thread.
				sigset_t new_mask;
				sigfillset(&new_mask);
				sigset_t old_mask;
				pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

				// Run server in background thread.
				doclib::daemon::server s(address, port, doc_root, num_threads);
				boost::thread t(boost::bind(&doclib::daemon::server::run, &s));

				// Restore previous signals.
				pthread_sigmask(SIG_SETMASK, &old_mask, 0);

				// Wait for signal indicating time to shut down.
				sigset_t wait_mask;
				sigemptyset(&wait_mask);
				sigaddset(&wait_mask, SIGINT);
				sigaddset(&wait_mask, SIGQUIT);
				sigaddset(&wait_mask, SIGTERM);
				pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
				int sig = 0;
				sigwait(&wait_mask, &sig);

				// Stop the server.
				s.stop();
				t.join();

#endif // !defined(_WIN32)

			}
			else
				LERR_ << "couldn't load document database";
	
			// we can destroy database singleton instance
			doclib::libmgr::doc_database::destroy_instance();
		}
		else
			cerr << "incorrect configuration (" << config_file << ")" << endl;
	}
	catch(std::exception &e)
	{
		LERR_ << "exception thrown : " << e.what();
	}

	return ret;
}
