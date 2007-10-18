#ifndef SVMMACHINETEST_HPP
#define SVMMACHINETEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  SVMMACHINE_TEST || defined TEST_ALL

#include "../SvmMachine.hpp"
#include "../SelfTestExperiment.hpp"
#include "../PreProcessor.hpp"
#include "../PreProcWorkUnit.hpp"
#include "../ExperimentResult.hpp"
#include "../Log.hpp"
#include "../CrossValidationExperiment.hpp"
#include "../Options.hpp"

#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct SvmMachineTest_Fixture
{
	PreProcessor p;
	Options preproc_options;
	
	//input
	PreProcWUSet input_set;

	//ctor
	SvmMachineTest_Fixture(){}

	SvmMachineTest_Fixture( std::string input_data_path, std::string drug )
	{
		p.parseInputFiles(
		input_data_path,
		"../data/shared/PI_wild.seq",
		drug,
		16,//threshold
		preproc_options,
		input_set
		);		
	}

	SvmMachineTest_Fixture( std::string input_data_path, std::string drug, double threshold )
	{
		p.parseInputFiles(
		input_data_path,
		"../data/shared/PI_wild.seq",
		drug,
		threshold,//threshold
		preproc_options,
		input_set
		);	

		Log::append( "SelfTestExperimentTest_Fixture input_set.size = " +  boost::lexical_cast<std::string>( input_set.size() )  );
	}

	//dtor
	~SvmMachineTest_Fixture(){}
};

BOOST_AUTO_TEST_SUITE( SvmMachineTests );

//NFV median is 16 for pr 23
BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_23_threshold0 )
{
	double threshold = 0;
	SvmMachineTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	SelfTestExperiment self_test_exp;

	//for grid search
	double log_cost_low			= -1;
	double log_cost_high			=  3;
	double log_cost_increment		=  2;

	double log_gamma_low			= -1;
	double log_gamma_high			=  3;
	double log_gamma_increment		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set(
		"SelfTest",
		"NFV",
		threshold,
		"../data/shared/PI_wild.seq",
		"../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv",
		f.input_set.size()
		);

	Options o;
	
	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		log_cost_low,
		log_cost_high,
		log_cost_increment,
		log_gamma_low,
		log_gamma_high,
		log_gamma_increment,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->log_cost, log_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_cost, log_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->log_gamma, log_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_gamma, log_gamma_high );

	//check for correct values
	for( size_t result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->datum.data_points.size(), f.input_set.size() );

		for( size_t i=0; i < f.input_set.size(); i++ )
		{
			
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 0. That means every sequence is predicted to be not_susceptible
			// so my guess is all the svm's predictions will be not_susceptible as well.
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->predicted_susceptibility,
				false );
		}
	}
}


BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_small23_threshold_10000 )
{
	double threshold = 10000;
	SvmMachineTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	SelfTestExperiment self_test_exp;

	//for grid search
	double log_cost_low			= -1;
	double log_cost_high			=  3;
	double log_cost_increment		=  2;

	double log_gamma_low			= -1;
	double log_gamma_high			=  3;
	double log_gamma_increment		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set(
		"SelfTest",
		"NFV",
		threshold,
		"../data/shared/PI_wild.seq",
		"../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv",
		f.input_set.size()
		);
	
	Options o;

	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		log_cost_low,
		log_cost_high,
		log_cost_increment,
		log_gamma_low,
		log_gamma_high,
		log_gamma_increment,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->log_cost, log_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_cost, log_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->log_gamma, log_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_gamma, log_gamma_high );

	//check for correct values
	for( size_t result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->datum.data_points.size(), f.input_set.size() );

		for( size_t i=0; i < f.input_set.size(); i++ )
		{	
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 100000. That means every sequence is predicted to be susceptible
			// so my guess is all the svm's predictions will be susceptible as well.
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->predicted_susceptibility,
				true );
		}
	}
}

	//yep, I'm going to need an experiment result class.
	//or should the Experiment hold it's results and expose them?
	//nope a separate class b/c the experiment just runs a single experiment.
	//However, ExperimentResult and ExperimentResulset may be needed for all the parameter searches.


#if defined  LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_full_threshold0 )
{
	double threshold = 0;
	SvmMachineTest_Fixture f( "../data/shared/PR_2006-05-25_v2.0.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	SelfTestExperiment self_test_exp;

	//for grid search
	double log_cost_low			= -1;
	double log_cost_high			=  3;
	double log_cost_increment		=  2;

	double log_gamma_low			= -1;
	double log_gamma_high			=  3;
	double log_gamma_increment		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set(
		"SelfTest",
		"NFV",
		threshold,
		"../data/shared/PI_wild.seq",
		"../data/shared/PR_2006-05-25_v2.0.tsv",
		f.input_set.size()
		);
	
	Options o;

	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		log_cost_low,
		log_cost_high,
		log_cost_increment,
		log_gamma_low,
		log_gamma_high,
		log_gamma_increment,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->log_cost, log_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_cost, log_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->log_gamma, log_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_gamma, log_gamma_high );

	//check for correct values
	for( size_t result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->datum.data_points.size(), f.input_set.size() );

		for( size_t i=0; i < f.input_set.size(); i++ )
		{
			
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 0. That means every sequence is predicted to be not_susceptible
			// so my guess is all the svm's predictions will be not_susceptible as well.
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->predicted_susceptibility,
				false );
		}
	}

}

#endif //#if defined LONG_TESTS || TEST_ALL 

BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_small23_threshold_10000_cross_val )
{
	double threshold = 10000;
	SvmMachineTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	CrossValidationExperiment cross_val_exp;

	//for grid search
	double log_cost_low				= -1;
	double log_cost_high			=  3;
	double log_cost_increment		=  2;

	double log_gamma_low			= -1;
	double log_gamma_high			=  3;
	double log_gamma_increment		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set(
		"Cross Validation",
		"NFV",
		threshold,
		"../data/shared/PI_wild.seq",
		"../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv",
		f.input_set.size()
		);
	
	Options o;

	machine.parameter_search( 
		cross_val_exp, 
		f.input_set,
		o,
		log_cost_low,
		log_cost_high,
		log_cost_increment,
		log_gamma_low,
		log_gamma_high,
		log_gamma_increment,
		result_set
		);

	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->log_cost, log_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_cost, log_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->log_gamma, log_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->log_gamma, log_gamma_high );

	//check for correct values
	for( size_t result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->datum.data_points.size(), f.input_set.size() );

		for( size_t i=0; i < f.input_set.size(); i++ )
		{	
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 100000. That means every sequence is predicted to be susceptible
			// so my guess is all the svm's predictions will be susceptible as well.
			BOOST_CHECK_EQUAL( er->datum.data_points.at(i)->predicted_susceptibility,
				true );
		}
	}
}


BOOST_AUTO_TEST_CASE( SvmMachineTest_run )
{
		//hivm --operation test --drug NFV --thresholds 2 10 

	int ac = 17;
	char* av[ 17 ];
	av[0] = "hivm";
	av[1] = "--purpose";
	av[2] = "prediction";
	av[3] = "--drug";
	av[4] = "NFV";
	av[5] = "--thresholds";
	av[6] = "2";
	av[7] = "--thresholds";
	av[8] = "10";

	av[9] = "-w";
	av[10] = "../data/shared/PI_wild.seq";

	av[11] = "-f";
	av[12] = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV25.tsv";

	av[13] = "-c";
	av[14] = "2";

	av[15] = "-g";
	av[16] = "3";

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  SVMMACHINE_TEST || defined TEST_ALL
#endif//SVMMACHINETEST_HPP
