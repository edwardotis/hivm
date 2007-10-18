#include "SvmMachine.hpp"
#include "Log.hpp"
#include "PreProcessor.hpp"
#include "ExperimentAnalyzer.hpp"
#include "ValidationExperiment.hpp"

#include <cmath>// floor function

#include <boost/lexical_cast.hpp> //safe numerical to string casts

//	//default constructor
SvmMachine::SvmMachine()
{}

//constructor
//SvmMachine::SvmMachine( Options& _options ) :
//options( _options )
//{}

	//default destructor
SvmMachine::~SvmMachine()
{}

void SvmMachine::run( const std::string purpose, Options& options  )
{
	if( purpose == "model-selection" )
	{
		model_select( options );
	}
	else if( purpose == "model-validation" )
	{
		model_validate( options );
	}
	else if( purpose == "prediction" )
	{
		predict( options );
	}
	else//problem. Log and Exit.
	{
		std::string msg;
		msg.append( "Error: Purpose is not valid: " + purpose );
		msg.append( "\nType hivm --help for usage. \nExiting..." );
		Log::append( msg );
		std::cerr << msg;
		return;
	}
}

/**
@purpose Place holder for future function. 
Predict should be created based on the use case for a doctor providing actual
unknown HIV sequences to hivm, and wanting appropriate prediction information back.
*/
void SvmMachine::predict( const Options& options )
{
	std::string msg;
	msg.append( "\n'prediction' is not yet implemented. This function will take HIV sequences of unknown " );
	msg.append( "susceptibility and return prediction information.\n" );
	Log::append( msg );
	std::cerr << msg;
	return;
}

void SvmMachine::model_select( Options& options )
{
	//initialize
	CrossValidationExperiment cross_val_exp;
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet result_set;
	ExperimentAnalyzer ea;

	Log::append( "Beginning Model Selection" );
	std::cerr << "\n\nBeginning Model Selection";

	std::cerr << "\n\nParsing File Input...";
	std::cerr << "\n(Please check the log for details of parsing.)";

	if( options.use_entire_susceptbility_file == 0 )
	{
		p.parseInputFiles(
			options.susceptibility_file,
			options.wild_type_file,
			options.drug,
			options.thresholds,//threshold
			options,
			options.seed,
			input_train_set,
			input_test_set
		);
	}
	else//use entire susceptibility file for model selection. seed ignored.
	{
		p.parseInputFiles(
			options.susceptibility_file,
			options.wild_type_file,
			options.drug,
			options.thresholds,//threshold
			options,
			input_train_set
		);
	}

	//TODO add Log entry

	this->parameter_search( 
		cross_val_exp, 
		input_train_set,
		options,
		options.lg_cost_low,
		options.lg_cost_high,
		options.lg_cost_inc,
		options.lg_gamma_low,
		options.lg_gamma_high,
		options.lg_gamma_inc,
		result_set
		);

	std::cerr << "\n\nFinished Grid Search.";
	std::cerr << "\n\nAnalyzing Results....";

	ea.analyze( result_set, options );
	ea.save_dataset_ids( input_train_set, "trainset", options );

}


void SvmMachine::model_validate( const Options& options )
{
	//initialize
	ValidationExperiment val_exp;
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet results;
	ExperimentAnalyzer ea;

	Log::append( "Beginning Model Validation" );
	std::cerr << "\n\nBeginning Model Validation";

	std::cerr << "\n\nParsing File Input...";
	std::cerr << "\n(Please check the log for details of parsing.)";

	if( options.susceptibility_file_test_dataset == "N/A" )//split susceptibility file with seed. 
	{
		p.parseInputFiles(
			options.susceptibility_file,
			options.wild_type_file,
			options.drug,
			options.thresholds,
			options,
			options.seed,
			input_train_set,
			input_test_set
		);
	}
	else //ignore seed. Use full train and full test files
	{
		//run parse single set 2 times.
		//1 for train. 
		p.parseInputFiles(
			options.susceptibility_file,
			options.wild_type_file,
			options.drug,
			options.thresholds,//threshold
			options,
			input_train_set
		);

		//1 for test.
		p.parseInputFiles(
			options.susceptibility_file_test_dataset,
			options.wild_type_file,
			options.drug,
			options.thresholds,//threshold
			options,
			input_test_set
		);
	}

	ExperimentResult* result;
	try
	{
		result = new ExperimentResult();
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in SvmMachine::model_validate new ExperimentResult: \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	std::cerr << "\n\nPerforming predictions...";
	val_exp.run_experiment( 
		input_train_set,
		input_test_set,
		options,
  		result
		);

	results.push_back( result );

	std::cerr << "\n\nAnalyzing Results....";

	ea.analyze( results, options );
	ea.save_dataset_ids( input_train_set, "trainset", options );

}



void SvmMachine::format_grid_search_output_( double lg_cost )
{
	if( lg_cost == floor( lg_cost ) )//integer
	{
		std::cerr << "  ";//adjust 2 spaces. assuming single decimal place used
	}

	if( lg_cost <= -10.0 )
	{}
	else if( lg_cost >= 0 && lg_cost < 10 )//adjust 2 spaces
	{
		std::cerr << "  ";
	}
	else//uses 2 spaces
	{
		std::cerr << " ";
	}
}

//TODO DRY
void SvmMachine::parameter_search( 
	CrossValidationExperiment& experiment,
	const PreProcWUSet& input_data,
	Options& svm_param_options,
	const double lg_cost_low,
	const double lg_cost_high,
	const double lg_cost_inc,
	const double lg_gamma_low,
	const double lg_gamma_high,
	const double lg_gamma_inc,
	ExperimentResultSet& results
	)
{
	double c;//cost
	double g;//gamma

	std::cerr << "\n\nRunning Grid Search on cost and gamma parameters.";
	std::cerr << "\nThis may take a while.";
	std::cerr << "\n\nEach line represents a lg cost value, followed by gamma values.\n";
	for( c = lg_cost_low; c <= lg_cost_high; c+=lg_cost_inc )
	{
		Log::append( "Using Cost: " + boost::lexical_cast<std::string>( c ) );
		std::cerr << "\n" + boost::lexical_cast<std::string>( c ) << ' ';

		format_grid_search_output_( c );

		for( g = lg_gamma_low; g <= lg_gamma_high; g+=lg_gamma_inc )
		{
			std::cerr << '.';

			//create new ExperimentResult pointer
			svm_param_options.lg_cost = c;
			svm_param_options.lg_gamma = g;
			
			ExperimentResult *result;
			try
			{
				result = new ExperimentResult();//memory handled by ExperimentResultSet
			}
			catch( std::exception& e )
			{
				std::string msg = "\nException caught in SvmMachine::parameter_search new ExperimentResult: \n";
				msg +=   e.what();
				msg += "\n\nElapsed Time: ";
				msg += Log::elapsed_time();
				msg += "\nExiting...";
				Log::append( msg );
				std::cerr << msg;
				exit(1);
			}

			
			try
			{
				experiment.run_experiment(input_data, result, svm_param_options ); 
			}
			catch( std::exception& e )
			{
				std::string msg = "\nException caught in SvmMachine::parameter_search call to CrossValidationExperiment::run_experiment(): \n";
				msg +=   e.what();
				msg += "\n Cost: ";
				msg +=  boost::lexical_cast<std::string>( c );
				msg += "\n Gamma: ";
				msg +=  boost::lexical_cast<std::string>( g );
				msg += "\n\nElapsed Time: ";
				msg += Log::elapsed_time();
				msg += "\nExiting...";
				Log::append( msg );
				std::cerr << msg;
				exit(1);
		}
			//save results to collection
			results.push_back( result );
		}
	}
}
