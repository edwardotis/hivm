#include "Options.hpp"
#include "Types.hpp"
#include "Log.hpp"
#include "FileIO.hpp"

#include <iostream>// cout
#include <fstream>//fstream
#include <algorithm>//std::copy
#include <iterator>//ostream iterator
#include <vector>

#include <boost/lexical_cast.hpp> //safe numerical to string casts
//#include <boost/filesystem/path.hpp>//FileIO error


// A helper function to simplify output of vector contents to stdout
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " ")); 
    return os;
}

namespace po = boost::program_options;

//default constructor JUST FOR UNIT TESTS!
Options::Options()
{
	int argc = 1;
	char* argv[1];
	argv[0] = "hivm";

	_initialize( argc, argv );
}

Options::Options( int argc, char* argv[] )
{
	_initialize( argc, argv );
}

void Options::_initialize( int argc, char* argv[] )
{
	//set other defaults here for now, but config file later
	// so that it's all handled in parse_options()
	
	//defaults here may be over written by cmdline in parse_options
	
	output_prefix = Log::get_start_time();

	//susceptibility_file = "../data/shared/PR_2006-05-25_v2.0.tsv";
	//wild_type_file = "../data/shared/PI_wild.seq";
    
	//did user ask for usage aka help?
	help_flag = false;

	//svm parameter
	svm_type	 = 0;
	kernel_type	 = 2;
	degree		 = 3.0;
	coef0		 = 0.0;
	nu			 = 0.5;//MATCHES LIBSVM PYTHON EXAMPLE 
	cache_size	 = 100;
	eps			 = 0.001;
	p			 = 0.1;//MATCHES LIBSVM PYTHON EXAMPLE
	shrinking	 = 1;
	nr_weight	 = 0;
	weight_label = NULL;
	weight		 = NULL;
	probability  = 0;//enables svm_predict to return probability of classes
	//svm_predict_probability()

	//vm is local instance of boost::program_options::variables_map
	parse_options( argc, argv, vm );

	//special case: set threshold(s) defaults
	//if( thresholds.empty() )
	//{
	//	thresholds.push_back( 2 );
	//	thresholds.push_back( 10 );
	//}

	std::sort( thresholds.begin(), thresholds.end(), std::less<double>() );

	//correct susceptiblity-type for capitalization of first letter to match suscep file
	if( susceptibility_type == "clinical" )
	{
		susceptibility_type = "Clinical";
	}

	if( susceptibility_type == "lab" )
	{
		susceptibility_type = "Lab";
	}

	if( susceptibility_type == "all" )//for consistency
	{
		susceptibility_type = "All";
	}
}

void Options::parse_options( int ac, char* av[], po::variables_map& vm )
{
	try
	{
		// Declare the supported options.
		po::options_description desc("Allowed options");
		desc.add_options()
		("help,h",		 "display usage")
		("purpose,p",    po::value<std::string>(&purpose), "model-selection or prediction")
		("drug,d",		 po::value<std::string>(&drug), "HIV drug to be tested")
		("thresholds,t", po::value<std::vector< double > >(&thresholds), "Thresholds for high  and low drug fold resistance . Please use only 1 or 2 thresholds. Default: 2 and 10")
		("wild-type,w",  po::value<std::string>(&wild_type_file), "Wild Type Enzyme Sequence File")
		("hivdb-file,f", po::value<std::string>(&susceptibility_file), "HIVDB Susceptibility Data File")		
		("seed,s",		 po::value<int>(&seed)->default_value(42), "Optional: Seed for randomly splitting susceptibility file into training and testing sets. Positive integers only. Default: 42")
		("suscep-type,e",po::value<std::string>(&susceptibility_type)->default_value("all"), "Optional: Type of susceptibility experiment used: clinical, lab, or all.")//TODO maybe just in config file
		("log-cost-c,c",		 po::value<double>(&log_cost)->default_value(0), "Log Cost.  Required: prediction operation. Ignored for CrossValidation operations")	
		("log-gamma-g,g",		 po::value<double>(&log_gamma)->default_value(0), "Log Gamma. Required: prediction operation. Ignored for CrossValidation operations")
		("log-cost-low,x",   po::value<double>(&log_cost_low)->default_value(0), "Low Log Cost. Required: model-selection. For Grid Search of Parameters")
		("log-cost-high,y",  po::value<double>(&log_cost_high)->default_value(0), "High Log Cost. Required: model-selection. For Grid Search of Parameters")
		("log-cost-increment,z", po::value<double>(&log_cost_increment)->default_value(0), "Log Cost Increment. Required: model-selection. For Grid Search of Parameters")
		("log-gamma-low,l",  po::value<double>(&log_gamma_low)->default_value(0), "Low Log Gamma. Required: model-selection. For Grid Search of Parameters")
		("log-gamma-high,m", po::value<double>(&log_gamma_high)->default_value(0), "High Log Gamma. Required: model-selection. For Grid Search of Parameters")
		("log-gamma-increment,n", po::value<double>(&log_gamma_increment)->default_value(0), "Log Gamma Increment. Required: model-selection. For Grid Search of Parameters")
		("output,o",	 po::value<std::string>(&output_prefix), "Optional: Prefix for output files. Default: current timestamp")
		//("config",	     po::value<std::string>(&hivm_config_file)->default_value("hivm_defaults.cfg"), "Optional: Custom configuration file. Default: hivm_defaulst.cfg")
		;

		//parse and store cmd line
		//po::variables_map vm;
		//po::store(po::parse_command_line(ac, av, desc), vm);
		po::store(po::command_line_parser(ac, av).options(desc).run(), vm);	
		po::notify(vm);

		//config file
		//std::ifstream ifs( hivm_config_file.c_str() );
  //      store(parse_config_file(ifs, config_file_options), vm);
  //      notify(vm);

		//display usage
		if (vm.count("help")) 
		{
			std::cout << desc << "\n";
			help_flag = true;
			return;
		}

	//scan for required entries to be set 
	//(and don't put in defaults for some of these required vars.)
	//if any required vars are not set, then print out help/usage again and exit.
	//depending on purpose, can give nice error messages here to user.
	

    }
	//TODO Log and exit here
	catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
    }

}
	

//destructor
Options::~Options(){}

po::variables_map& Options::get_options()
{
	return vm;
}

//TODO add the line breaks in programmtically here DRY
void Options::save_cmd_line_options(  ) const
{
	//should be different based on the purpose (operation) used in the run
	//b/c don't need paramsearch variables for test, train or classify
	
	std::string cmdline;//for cut and paste back into a script
	std::string opts;//for legibility

	//put in start time and elapsed time
	opts.append( "Start Time: " + Log::get_start_time() + "\n");
	opts.append( Log::elapsed_time() + "\n\n");
	
	cmdline.append( "\nhivm " );

	if( purpose == "model-selection" )//ie. cross-validation
	{
		opts.append(    "--purpose model-selection\n" );
		cmdline.append( "--purpose model-selection " );
	}
	else if( purpose == "self-validate" )
	{
		opts.append(    "--purpose self-validate\n" );
		cmdline.append( "--purpose self-validate " );
	}
	else if( purpose == "prediction" )// i.e. test
	{
		opts.append(    "--purpose prediction\n" );
		cmdline.append( "--purpose prediction " );	
	}
	//stub for train and classify

	//common to all
	if( purpose == "model-selection" || purpose == "self-validate" ||  purpose == "prediction" )
	{
		opts.append( "--drug " + drug + "\n" );
		
		for( size_t i=0; i < thresholds.size(); i++ )
		{
			opts.append( "--thresholds  " + boost::lexical_cast<std::string>( thresholds[i] ) + "\n" );
		}
		
		opts.append( "--wild-type " + wild_type_file + "\n" );
		opts.append( "--hivdb-file " + susceptibility_file + "\n" );
		opts.append( "--seed " + boost::lexical_cast<std::string>( seed ) + "\n" );
		opts.append( "--suscep-type " + susceptibility_type + "\n" );
		opts.append( "--output " + output_prefix + "\n" );
		//opts.append( "--config " + hivm_config_file + "\n" );

		//cmd line
		cmdline.append( "--drug " + drug + " " );
		
		for( size_t i=0; i < thresholds.size(); i++ )
		{
			cmdline.append( "--thresholds  " + boost::lexical_cast<std::string>( thresholds[i] ) + " " );
		}
		
		cmdline.append( "--wild-type " + wild_type_file + " " );
		cmdline.append( "--hivdb-file " + susceptibility_file + " " );
		cmdline.append( "--seed " + boost::lexical_cast<std::string>( seed ) + " " );
		cmdline.append( "--suscep-type " + susceptibility_type + " " );
		cmdline.append( "--output " + output_prefix + " " );
		//cmdline.append( "--config " + hivm_config_file + " " );

	}

	//param search options
	if( purpose == "model-selection" || purpose == "self-validate" )
	{
		opts.append( "--log-cost-low " + boost::lexical_cast<std::string>( log_cost_low ) + "\n" );
		opts.append( "--log-cost-high " + boost::lexical_cast<std::string>( log_cost_high ) + "\n" );
		opts.append( "--log-cost-increment " + boost::lexical_cast<std::string>( log_cost_increment ) + "\n" );
		opts.append( "--log-gamma-low " + boost::lexical_cast<std::string>( log_gamma_low ) + "\n" );
		opts.append( "--log-gamma-high " + boost::lexical_cast<std::string>( log_gamma_high ) + "\n" );
		opts.append( "--log-gamma-increment " + boost::lexical_cast<std::string>( log_gamma_increment ) + "\n" );
		
		cmdline.append( "--log-cost-low " + boost::lexical_cast<std::string>( log_cost_low ) + " " );
		cmdline.append( "--log-cost-high " + boost::lexical_cast<std::string>( log_cost_high ) + " " );
		cmdline.append( "--log-cost-increment " + boost::lexical_cast<std::string>( log_cost_increment ) + " " );
		cmdline.append( "--log-gamma-low " + boost::lexical_cast<std::string>( log_gamma_low ) + " " );
		cmdline.append( "--log-gamma-high " + boost::lexical_cast<std::string>( log_gamma_high ) + " " );
		cmdline.append( "--log-gamma-increment " + boost::lexical_cast<std::string>( log_gamma_increment ) + " " );
	}

	if( purpose == "prediction" )
	{
		opts.append( "--log-cost-c " + boost::lexical_cast<std::string>( log_cost ) + "\n" );
		opts.append( "--log-gamma-g " + boost::lexical_cast<std::string>( log_gamma ) + "\n" );

		cmdline.append( "--log-cost-c " + boost::lexical_cast<std::string>( log_cost ) + " " );
		cmdline.append( "--log-gamma-g " + boost::lexical_cast<std::string>( log_gamma ) + " " );
	}

	//TODO
	//SPECIAL CASE BEFORE IMPLEMENTING CONFIG FILES
	//SAVING SVM PARAMS HERE THAT ARE SET IN CONSTRUCTOR
	opts.append( "\n\nSvm Parameters: Set in Options.hpp constructor\n" );
	opts.append( "TODO: Move this to a config file.\n" );
	opts.append( "svm_type	 = 0\n");
	opts.append( "kernel_type	 = 2\n");
	opts.append( "degree		 = 3.0\n");
	opts.append( "coef0		 = 0.0\n");
	opts.append( "nu			 = 0.5\n");//MATCHES LIBSVM PYTHON EXAMPLE 
	opts.append( "cache_size	 = 100\n");
	opts.append( "eps			 = 0.001\n");
	opts.append( "p			 = 0.1\n");//MATCHES LIBSVM PYTHON EXAMPLE
	opts.append( "shrinking	 = 1\n");
	opts.append( "nr_weight	 = 0\n");
	opts.append( "weight_label = NULL\n");
	opts.append( "weight		 = NULL\n");
	opts.append( "probability  = 0\n");

//now save to file
	std::string file_name;
	file_name.append( output_prefix + "_cmdline.txt" );

	try
	{
		FileIO::write( file_name, opts );
		FileIO::append( file_name, cmdline );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr <<  e.what() << std::endl;
		std::cerr << "Could not open file for writing." << std::endl;
		std::cerr << "File name: " << file_name << std::endl;
		std::cerr << "Outputing to std::err instead\n";
		std::cerr << opts;
		std::cerr << cmdline;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
	
}


