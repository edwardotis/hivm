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
		options.log_cost_low,
		options.log_cost_high,
		options.log_cost_increment,
		options.log_gamma_low,
		options.log_gamma_high,
		options.log_gamma_increment,
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
		options.log_cost_low,
		options.log_cost_high,
		options.log_cost_increment,
		options.log_gamma_low,
		options.log_gamma_high,
		options.log_gamma_increment,
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
	ExperimentResultSet result_set;
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

	//ExperimentResult* result = new ExperimentResult(
	//	options.log_cost, options.log_gamma
	//	);

	val_exp.run_experiment( 
		input_train_set,
		input_test_set,
		options,
  		result_set
		);

//	result_set.push_back( result );

	ea.analyze( result_set, options );

}

//TODO DRY
void SvmMachine::parameter_search( 
	SelfTestExperiment& experiment,
	const PreProcWUSet& input_data,
	Options& svm_param_options,
	const double log_cost_low,
	const double log_cost_high,
	const double log_cost_increment,
	const double log_gamma_low,
	const double log_gamma_high,
	const double log_gamma_increment,
	ExperimentResultSet& result_set
	)
{
	double c;//cost
	double g;//gamma

	Log::append( "Self Test Experiment" );
	//Log::append( "Accuracy\t\tLog Cost\t\tLog Gamma" );
	
	std::cerr << "\n";
	for( c = log_cost_low; c <= log_cost_high; c+=log_cost_increment )
	{
		std::cerr << "\n";
		for( g = log_gamma_low; g <= log_gamma_high; g+=log_gamma_increment )
		{
			std::cerr << ".";

			//create new ExperimentResult pointer
			//ExperimentResult *result = new ExperimentResult( c, g );
			svm_param_options.log_cost = c;
			svm_param_options.log_gamma = g;

			//run experiment
			experiment.run_experiment(input_data, result_set, svm_param_options ); 

			//save results to collection
			//result_set.push_back( result );
		}
	}
}

//TODO DRY
void SvmMachine::parameter_search( 
	CrossValidationExperiment& experiment,
	const PreProcWUSet& input_data,
	Options& svm_param_options,
	const double log_cost_low,
	const double log_cost_high,
	const double log_cost_increment,
	const double log_gamma_low,
	const double log_gamma_high,
	const double log_gamma_increment,
	ExperimentResultSet& result_set
	)
{
	double c;//cost
	double g;//gamma

	Log::append( "Cross Validation Experiment" );
	//Log::append( "Accuracy\t\tLog Cost\t\tLog Gamma" );
	
	std::cerr << "\n";
	for( c = log_cost_low; c <= log_cost_high; c+=log_cost_increment )
	{
		std::cerr << "\n";
		for( g = log_gamma_low; g <= log_gamma_high; g+=log_gamma_increment )
		{
			std::cerr << ".";

			//create new ExperimentResult pointer
			//ExperimentResult *result = new ExperimentResult( c, g );
			svm_param_options.log_cost = c;
			svm_param_options.log_gamma = g;

			//run experiment
			experiment.run_experiment(input_data, result_set, svm_param_options ); 

			//save results to collection
			//result_set.push_back( result );
		}
	}
}
