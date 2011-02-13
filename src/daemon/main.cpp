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

#include "doclibd_includes.hpp"
#include "doclibd.hpp"
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

# include <pthread.h>
# include <signal.h>

using namespace std;

// local instances
doclib::core::doclib_conf cfg;


int main(int argc, char ** argv)
{
	int ret = EXIT_FAILURE;

	try
	{
		doclib::daemon::commandline_options options;
		cout << "DocumentLibrary version " << doclib::daemon::get_version_string() << endl;
		treat_command_line(argc, argv, options);

		if (options.treatment == doclib::daemon::clt_success && cfg.load(options.config_file))
		{
			// init log instance, providing it 'ini file loaded' values
			doclib::core::core_services::init_logging(cfg);
			LDBG_ << "successfully loaded configuration file";

			doclib::libmgr::doc_database::create_instance(cfg);

			if (doclib::libmgr::doc_database::get_instance().load_database())
			{
				LDBG_ << "successfully loaded document database";
				ret = EXIT_SUCCESS;


				// load daemon server parameters
				string address = "0.0.0.0";
				string port = boost::lexical_cast<std::string>(cfg.get_port());
				std::size_t num_threads = cfg.get_num_threads();

				// block signal handling until server is started
				sigset_t new_mask;
				sigfillset(&new_mask);
				sigset_t old_mask;
				pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

				// Run server in background thread.
				doclib::daemon::server s(address, port, num_threads);
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
				L_ << "server stopped" << std::endl;
				t.join();
				LDBG_ << "background thread stopped" << std::endl;

			}
			else
				LERR_ << "couldn't load document database";
	
			// we can destroy database singleton instance
			doclib::libmgr::doc_database::destroy_instance();
		}
		else
			cerr << "incorrect configuration (" << options.config_file << ")" << endl;
	}
	catch(std::exception &e)
	{
		LERR_ << "exception thrown : " << e.what() << endl;
		cerr << "exception thrown : " << e.what() << endl;
	}

	return ret;
}
