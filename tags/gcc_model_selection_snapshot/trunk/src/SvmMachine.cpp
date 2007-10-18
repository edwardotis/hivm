#include "SvmMachine.hpp"
#include "Log.hpp"
#include "PreProcessor.hpp"
#include "ExperimentAnalyzer.hpp"
#include "ValidationExperiment.hpp"


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
		cross_validate( options );
	}
	else if( purpose == "self-validate" )
	{
		self_validate( options );
	}
	else if( purpose == "prediction" )
	{
		test( options );
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

void SvmMachine::cross_validate( Options& options )
{
	//initialize
	CrossValidationExperiment cross_val_exp;
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet result_set;
	ExperimentAnalyzer ea;

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

	ea.analyze( result_set, options );

}

void SvmMachine::self_validate( Options& options  )
{
	//initialize
	SelfTestExperiment self_val_exp;
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet result_set;
	ExperimentAnalyzer ea;

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

	//TODO add Log entry

	this->parameter_search( 
		self_val_exp, 
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

	ea.analyze( result_set, options );
}

void SvmMachine::test( const Options& options )
{
	//initialize
	ValidationExperiment val_exp;
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet results;
	ExperimentAnalyzer ea;

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

	ExperimentResult* result = new ExperimentResult();

	val_exp.run_experiment( 
		input_train_set,
		input_test_set,
		options,
  		result
		);

	results.push_back( result );

	ea.analyze( results, options );

}

//TODO DRY
void SvmMachine::parameter_search( 
	SelfTestExperiment& experiment,
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

	Log::append( "Self Test Experiment" );
	//Log::append( "Accuracy\t\tLog Cost\t\tLog Gamma" );
	
	std::cerr << "\n";
	for( c = lg_cost_low; c <= lg_cost_high; c+=lg_cost_inc )
	{
		std::cerr << "\n";
		for( g = lg_gamma_low; g <= lg_gamma_high; g+=lg_gamma_inc )
		{
			std::cerr << ".";

			//create new ExperimentResult pointer
			svm_param_options.lg_cost = c;
			svm_param_options.lg_gamma = g;

			ExperimentResult *result = new ExperimentResult();//memory handled by ExperimentResultSet
			experiment.run_experiment(input_data, result, svm_param_options ); 

			//save results to collection
			results.push_back( result );
		}
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

	Log::append( "Cross Validation Experiment" );
	//Log::append( "Accuracy\t\tLog Cost\t\tLog Gamma" );
	std::cerr << "\n";
	for( c = lg_cost_low; c <= lg_cost_high; c+=lg_cost_inc )
	{
		Log::append( "Using Cost: " + boost::lexical_cast<std::string>( c ) );
		std::cerr << "\n" + boost::lexical_cast<std::string>( c );
		for( g = lg_gamma_low; g <= lg_gamma_high; g+=lg_gamma_inc )
		{
			//std::cerr << boost::lexical_cast<std::string>( g );
			std::cerr << ".";

			//create new ExperimentResult pointer
			svm_param_options.lg_cost = c;
			svm_param_options.lg_gamma = g;

			ExperimentResult *result = new ExperimentResult();//memory handled by ExperimentResultSet
			
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


	std::cerr << "\n\nHolding program open to see how much memory is in use. Type in a character and 'enter', to exit.\n";
	char wait;
	std::cin >> wait; 

			exit(1);
		}


			//save results to collection
			results.push_back( result );
		}
	}
}
