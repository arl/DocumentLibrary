#include "doclibd_includes.hpp"
#include "../core/doclib_core.hpp"
#include "../libmgr/doc_database.hpp"
#include "../version.hpp"


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
	set_version_string();
	cout << "DocumentLibrary version " << version_str << endl;
	treat_command_line(argc, argv);

	if (cfg.load(config_file))
	{
		// init log instance, providing it with ini-loaded values
		doclib::core::core_services::init_logging(cfg.log_filename, cfg.log_level);
		LDBG_ << "successfully loaded configuration file";
		
		doclib::libmgr::doc_database::create_instance(cfg);

		if (doclib::libmgr::doc_database::get_instance().load_database())
		{
			LDBG_ << "successfully loaded document database";
			return EXIT_SUCCESS;
		}
		LERR_ << "couldn't load document database";
	}
	else
		cerr << "incorrect configuration (" << config_file << ")" << endl;

	return EXIT_FAILURE;

}
