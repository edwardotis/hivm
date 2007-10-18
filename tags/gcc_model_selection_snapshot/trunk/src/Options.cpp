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
	//set other defaults here for now, but defaults should be in config file later
	// so that it's all handled in parse_options()
	//defaults here may be over written by cmdline in parse_options
	
	//REQUIRED CMD LINE OPTIONS
	drug = "";

	//SET DEFAULTS FOR CMD LINE OPTIONAL PARAMS HERE
	//i.e. options that don't have to be set by user at cmd line.

	//Shared Options
	help_flag = false;//did user ask for usage aka help? -h, --help
	purpose = "model-selection";
	theshold_default = 10;
	wild_type_file = "../data/PI_wild.seq";
	susceptibility_file = "../data/PR_2006-05-25_v2.0.tsv";
	seed = 42;
	susceptibility_type = "all";
	probability  = 1;//enables svm_predict_probability() to return probability of classes

	//todo optimize these defaults by looking at previous results.
	//Prediction Only
	lg_cost = 14;
	lg_gamma = -7.5;

	//model-selection only
	lg_cost_low = -16;//0
	lg_cost_high = 16;
	lg_cost_inc = 2;

	lg_gamma_low = -16;
	lg_gamma_high = 16;//16
	lg_gamma_inc = 2;

	//time stamp provides unique id for output so that defaults never cause user to lose
	//data due to automatic overwriting of previous results.
	output_prefix = Log::get_start_time();


	//svm parameter defaults
	svm_type	 = 0;
	kernel_type	 = 2;
	degree		 = 3;
	coef0		 = 0.0;
	nu			 = 0.5;//MATCHES LIBSVM PYTHON EXAMPLE 
	cache_size	 = 100;//100;
	eps			 = 0.001;
	p			 = 0.1;//MATCHES LIBSVM PYTHON EXAMPLE
	shrinking	 = 1;
	nr_weight	 = 0;
	weight_label = NULL;
	weight		 = NULL;


	//vm is local instance of boost::program_options::variables_map
	parse_options( argc, argv, vm );

	//logic section begins here for params that are require some processing
	//before being used.

	std::sort( thresholds.begin(), thresholds.end(), std::less<double>() );

	//correct susceptiblity-type for capitalization of first letter to match suscep file
	if( susceptibility_type == "clinical" || 
		susceptibility_type == "Clinical" )
	{
		susceptibility_type = "Clinical";
	}

	if( susceptibility_type == "lab" || 
		susceptibility_type == "Lab" )
	{
		susceptibility_type = "Lab";
	}

	if( susceptibility_type == "all" || 
		susceptibility_type == "All" )//for consistency
	{
		susceptibility_type = "All";
	}

}

bool Options::validate_required_options_()
{
	bool are_options_valid = true;
	std::string msg = "\nMissing required parameters(s):\n";

	if( drug == "" )
	{
		are_options_valid = false;
		msg.append( "drug\n" );	
	}

	if( !are_options_valid )
	{
		std::cerr << msg << '\n';
		Log::append( msg );
	}

	return are_options_valid;
}

void Options::parse_options( int ac, char* av[], po::variables_map& vm )
{
	try
	{
		//Required Parameter descriptions
		std::string drug_desc = "HIV drug to be tested";
		//thresholds

		// Declare the required parameters
		po::options_description required("Required Parameters");
		required.add_options()
		("drug,d",		 po::value<std::string>(&drug), "HIV drug to be tested")
		;

		//Shared Options descriptions w/ defaults
		std::string purpose_desc = "model-selection or prediction [" + purpose + "]";
		std::string thresholds_desc = "Low and high thresholds for drug fold resistance.\n\tPlease use only one or two values. ";
		thresholds_desc += "[" + boost::lexical_cast<std::string>( theshold_default ) + "]";
		std::string wild_type_desc = "Wild Type Enzyme Sequence File [" + wild_type_file + "]";
		std::string hivdb_file_desc = "HIVDB Susceptibility Data File [" + susceptibility_file + "]";
		std::string output_desc = "Prefix for output files. ['timestamp']";
		std::string seed_desc = "Seed for training/test set partition, pos. integer [" + boost::lexical_cast<std::string>(seed) + "]";
		std::string suscep_type_desc = "Type of susceptibility (clinical, lab, all) [" + susceptibility_type + "]";
		std::string probability_desc = "Adds probability info. Changes SVM predicted class. model-selection and prediction should synchronzie this option. (0,1) [" + boost::lexical_cast<std::string>(probability) + "]";


		//Declare the shared options
		po::options_description shared("Shared Options");
		shared.add_options()
		("help,h",		  "display usage")
		("purpose,p",     po::value<std::string>(&purpose), purpose_desc.c_str() )
		("thresholds,t",  po::value<std::vector< double > >(&thresholds), thresholds_desc.c_str() )
		("wild-type,w",   po::value<std::string>(&wild_type_file), wild_type_desc.c_str() )
		("hivdb-file,f",  po::value<std::string>(&susceptibility_file), hivdb_file_desc.c_str() )		
		("output,o",	  po::value<std::string>(&output_prefix), output_desc.c_str() )
		("seed,s",		  po::value<int>(&seed), seed_desc.c_str() )
		("suscep-type,e", po::value<std::string>(&susceptibility_type), suscep_type_desc.c_str() )//TODO maybe just in config file
		("probability,b", po::value<int>(&probability), probability_desc.c_str() )
		//("config",	  po::value<std::string>(&hivm_config_file)->default_value("hivm_defaults.cfg"), "Optional: Custom configuration file. Default: hivm_defaulst.cfg")
		;

		//Prediction Only Options descriptions w/ defaults
		std::string lg_cost_c_desc =  "SVM lg(cost) parameter.  Good choice is critical to performance. See Readme.txt [" + boost::lexical_cast<std::string>( lg_cost ) + "]";
		std::string lg_gamma_g_desc = "SVM lg(gamma) parameter. Good choice is critical to performance. See Readme.txt [" + boost::lexical_cast<std::string>( lg_gamma ) + "]";

		po::options_description prediction("Prediction Only Options");
		prediction.add_options()
		("lg-cost-c,c",		 po::value<double>(&lg_cost),  lg_cost_c_desc.c_str() )//todo rename to lg-cost, or was there an issue?
		("lg-gamma-g,g",	 po::value<double>(&lg_gamma), lg_gamma_g_desc.c_str() )//todo rename to lg-gamma	
		;

		//Model-Selection Only  Options descriptions w/ defaults
		std::string lg_cost_low_desc =   "SVM lg(cost) parameter, grid search low bound [" + boost::lexical_cast<std::string>( lg_cost_low ) + "]";
		std::string lg_cost_high_desc =  "SVM lg(cost) parameter, grid search high bound [" + boost::lexical_cast<std::string>( lg_cost_high ) + "]";
		std::string lg_cost_inc_desc =   "SVM lg(cost) parameter, grid search increment [" + boost::lexical_cast<std::string>( lg_cost_inc ) + "]";
		std::string lg_gamma_low_desc =  "SVM lg(gamma) parameter, grid search low bound [" + boost::lexical_cast<std::string>( lg_gamma_low ) + "]";
		std::string lg_gamma_high_desc = "SVM lg(gamma) parameter, grid search high bound [" + boost::lexical_cast<std::string>( lg_gamma_high ) + "]";
		std::string lg_gamma_inc_desc =  "SVM lg(gamma) parameter, grid search increment [" + boost::lexical_cast<std::string>( lg_gamma_inc ) + "]";	
		
		//Describe Parameter Search Options
		po::options_description model_selection("Model-Selection Only Options.\nGrid Search for Optimal Parameters");
		model_selection.add_options()
		("lg-cost-low,x",   po::value<double>(&lg_cost_low),   lg_cost_low_desc.c_str() )
		("lg-cost-high,y",  po::value<double>(&lg_cost_high),  lg_cost_high_desc.c_str() )
		("lg-cost-inc,z", po::value<double>(&lg_cost_inc),     lg_cost_inc_desc.c_str() )
		("lg-gamma-low,l",  po::value<double>(&lg_gamma_low),  lg_gamma_low_desc.c_str()  )
		("lg-gamma-high,m", po::value<double>(&lg_gamma_high), lg_gamma_high_desc.c_str()  )
		("lg-gamma-inc,n", po::value<double>(&lg_gamma_inc),   lg_gamma_inc_desc.c_str()  )
		;

		//must combine all the cmd line option descriptors here
		po::options_description visible("Visible options");
		visible.add(required);
		visible.add(shared);
		visible.add(prediction);
		visible.add(model_selection);

		//parse and store cmd line
		//po::variables_map vm;
		//po::store(po::parse_command_line(ac, av, desc), vm);
		po::store(po::command_line_parser(ac, av).options(visible).run(), vm);	
		//po::store(po::command_line_parser(ac, av).options(visible).options(params).run(), vm);	
		//po::store(po::command_line_parser(ac, av).options(optional).run(), vm);	
		po::notify(vm);

		//config file
		//std::ifstream ifs( hivm_config_file.c_str() );
  //      store(parse_config_file(ifs, config_file_options), vm);
  //      notify(vm);

		//Usage description
		std::string usage = "\nUsage: hivm [OPTION]... \n";
		usage += "Example: hivm -d NFV -t 2 -o myoutput_ \n";

		//display usage if requested or if no cmd line options passed
		//Unit Tests frequently have no cmd line args, so disable this feature
		//when unit testing.
		#if defined UNIT_TESTING
			if ( vm.count("help") ) 
		#else
			if ( vm.count("help")   ) 
		#endif	
		{
			std::cout << usage << "\n";
			std::cout << required << "\n";
			std::cout << shared << "\n";
			std::cout << prediction << "\n";
			std::cout << model_selection << "\n";
			help_flag = true;
			return;
		}

	//scan for required entries to be set 
	//(and don't put in defaults for some of these required vars.)
	//if any required vars are not set, then print out help/usage again and exit.
	//depending on purpose, can give nice error messages here to user.

	#if !defined UNIT_TESTING
		if( !validate_required_options_() )
		{
			std::cout << "Please type 'hivm -h' for help with usage.\n";
			help_flag = true;//tells main to exit immediately.
			return;
		}
	#endif

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

void Options::save_cmd_line_options() const
{
	save_cmd_line_options_("");
}

void Options::save_cmd_line_options( const std::string file_name_prefix ) const
{
	save_cmd_line_options_( file_name_prefix );
}

//TODO add the line breaks in programmtically here DRY
void Options::save_cmd_line_options_( const std::string file_name_prefix  ) const
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
		
		for( int i=0; i < thresholds.size(); i++ )
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
		
		for( int i=0; i < thresholds.size(); i++ )
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
		opts.append( "--lg-cost-low " + boost::lexical_cast<std::string>( lg_cost_low ) + "\n" );
		opts.append( "--lg-cost-high " + boost::lexical_cast<std::string>( lg_cost_high ) + "\n" );
		opts.append( "--lg-cost-inc " + boost::lexical_cast<std::string>( lg_cost_inc ) + "\n" );
		opts.append( "--lg-gamma-low " + boost::lexical_cast<std::string>( lg_gamma_low ) + "\n" );
		opts.append( "--lg-gamma-high " + boost::lexical_cast<std::string>( lg_gamma_high ) + "\n" );
		opts.append( "--lg-gamma-inc " + boost::lexical_cast<std::string>( lg_gamma_inc ) + "\n" );
		
		cmdline.append( "--lg-cost-low " + boost::lexical_cast<std::string>( lg_cost_low ) + " " );
		cmdline.append( "--lg-cost-high " + boost::lexical_cast<std::string>( lg_cost_high ) + " " );
		cmdline.append( "--lg-cost-inc " + boost::lexical_cast<std::string>( lg_cost_inc ) + " " );
		cmdline.append( "--lg-gamma-low " + boost::lexical_cast<std::string>( lg_gamma_low ) + " " );
		cmdline.append( "--lg-gamma-high " + boost::lexical_cast<std::string>( lg_gamma_high ) + " " );
		cmdline.append( "--lg-cost-inc " + boost::lexical_cast<std::string>( lg_gamma_inc ) + " " );
	}

	if( purpose == "prediction" )
	{
		opts.append( "--lg-cost-c " + boost::lexical_cast<std::string>( lg_cost ) + "\n" );
		opts.append( "--lg-gamma-g " + boost::lexical_cast<std::string>( lg_gamma ) + "\n" );

		cmdline.append( "--lg-cost-c " + boost::lexical_cast<std::string>( lg_cost ) + " " );
		cmdline.append( "--lg-gamma-g " + boost::lexical_cast<std::string>( lg_gamma ) + " " );
	}

	//TODO
	//SPECIAL CASE BEFORE IMPLEMENTING CONFIG FILES
	//SAVING SVM PARAMS HERE THAT ARE SET IN CONSTRUCTOR
	opts.append( "\n\nSvm Parameters: Set in Options.hpp constructor\n" );
	opts.append( "TODO: Move this to a config file.\n" );
	opts.append( "svm_type		 = " + boost::lexical_cast<std::string>(svm_type) + "\n");
	opts.append( "kernel_type	 = " + boost::lexical_cast<std::string>(kernel_type) + "\n");
	opts.append( "degree		 = " + boost::lexical_cast<std::string>(degree) + "\n");
	opts.append( "coef0			 = " + boost::lexical_cast<std::string>(coef0) + "\n");
	opts.append( "nu			 = " + boost::lexical_cast<std::string>(nu) + "\n");//MATCHES LIBSVM PYTHON EXAMPLE 
	opts.append( "cache_size	 = " + boost::lexical_cast<std::string>(cache_size) + "\n");
	opts.append( "eps			 = " + boost::lexical_cast<std::string>(eps) + "\n");
	opts.append( "p				 = " + boost::lexical_cast<std::string>(p) + "\n");//MATCHES LIBSVM PYTHON EXAMPLE
	opts.append( "shrinking		 = " + boost::lexical_cast<std::string>(shrinking) + "\n");
	opts.append( "nr_weight		 = " + boost::lexical_cast<std::string>(nr_weight) + "\n");
	opts.append( "weight_label   = " + boost::lexical_cast<std::string>(weight_label) + "\n");
	opts.append( "weight		 = " + boost::lexical_cast<std::string>(weight) + "\n");
	opts.append( "probability    = " + boost::lexical_cast<std::string>(probability) + "\n");

//now save to file
	std::string file_name;

	if( file_name_prefix == "" )
	{
		file_name.append( output_prefix + "_cmdline.txt" );
	}
	else
	{
		file_name.append( file_name_prefix + "_cmdline.txt" );
	}

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


