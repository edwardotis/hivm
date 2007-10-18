#ifndef SVMMACHINETEST_HPP
#define SVMMACHINETEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  SVMMACHINE_TEST || defined TEST_ALL

#include "../SvmMachine.hpp"
#include "../PreProcessor.hpp"
#include "../PreProcWorkUnit.hpp"
#include "../ExperimentResult.hpp"
#include "../Log.hpp"
#include "../CrossValidationExperiment.hpp"
#include "../Options.hpp"

#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

class SvmMachineTest
{

public:
	
	//default ctor
	SvmMachineTest()
	{}

	//ctor
	SvmMachineTest( std::string input_data_path, std::string drug )
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

	//ctor
	SvmMachineTest( std::string input_data_path, std::string drug, double threshold )
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
	~SvmMachineTest()
	{}

void pr_23_threshold0()
{
	double threshold = 0;
	SvmMachineTest f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	CrossValidationExperiment self_test_exp;

	//for grid search
	double lg_cost_low			= -1;
	double lg_cost_high			=  3;
	double lg_cost_inc		=  2;

	double lg_gamma_low			= -1;
	double lg_gamma_high			=  3;
	double lg_gamma_inc		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set;

	Options o;
	
	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		lg_cost_low,
		lg_cost_high,
		lg_cost_inc,
		lg_gamma_low,
		lg_gamma_high,
		lg_gamma_inc,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->lg_cost, lg_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_cost, lg_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, lg_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, lg_gamma_high );

	//check for correct values
	for( int result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->size(), f.input_set.size() );

		for( int i=0; i < f.input_set.size(); i++ )
		{
			
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 0. That means every sequence is predicted to be not_susceptible
			// so my guess is all the svm's predictions will be not_susceptible as well.
			BOOST_CHECK_EQUAL( er->at(i)->predicted_susceptibility,
				false );
		}
	}
}



void pr_full_threshold0()
{
	double threshold = 0;
	SvmMachineTest f( "../data/shared/PR_2006-05-25_v2.0.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	CrossValidationExperiment self_test_exp;

	//for grid search
	double lg_cost_low			= -1;
	double lg_cost_high			=  3;
	double lg_cost_inc		=  2;

	double lg_gamma_low			= -1;
	double lg_gamma_high			=  3;
	double lg_gamma_inc		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set;
	
	Options o;

	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		lg_cost_low,
		lg_cost_high,
		lg_cost_inc,
		lg_gamma_low,
		lg_gamma_high,
		lg_gamma_inc,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->lg_cost, lg_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_cost, lg_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, lg_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, lg_gamma_high );

	//check for correct values
	for( int result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->size(), f.input_set.size() );

		for( int i=0; i < f.input_set.size(); i++ )
		{
			
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 0. That means every sequence is predicted to be not_susceptible
			// so my guess is all the svm's predictions will be not_susceptible as well.
			BOOST_CHECK_EQUAL( er->at(i)->predicted_susceptibility,
				false );
		}
	}


}



void pr_small23_threshold_10000()
{

	double threshold = 10000;
	SvmMachineTest f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	CrossValidationExperiment self_test_exp;

	//for grid search
	double lg_cost_low			= -1;
	double lg_cost_high			=  3;
	double lg_cost_inc		=  2;

	double lg_gamma_low			= -1;
	double lg_gamma_high			=  3;
	double lg_gamma_inc		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set;/*(
		"SelfTest",
		"NFV",
		threshold,
		"../data/shared/PI_wild.seq",
		"../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv",
		f.input_set.size()
		);*/
	
	Options o;

	machine.parameter_search( 
		self_test_exp, 
		f.input_set,
		o,
		lg_cost_low,
		lg_cost_high,
		lg_cost_inc,
		lg_gamma_low,
		lg_gamma_high,
		lg_gamma_inc,
		result_set
		);


	//TODO make more tests
	//tests
	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->lg_cost, lg_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_cost, lg_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, lg_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, lg_gamma_high );

	//check for correct values
	for( int result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->size(), f.input_set.size() );

		for( int i=0; i < f.input_set.size(); i++ )
		{	
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 100000. That means every sequence is predicted to be susceptible
			// so my guess is all the svm's predictions will be susceptible as well.
			BOOST_CHECK_EQUAL( er->at(i)->predicted_susceptibility,
				true );
		}
	}

}


void pr_small23_threshold_10000_cross_val()
{
	double threshold = 10000;
	SvmMachineTest f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", 
		"NFV", threshold );		
	SvmMachine machine;
	CrossValidationExperiment cross_val_exp;

	//for grid search
	double lg_cost_low				= -1;
	double lg_cost_high			=  3;
	double lg_cost_inc		=  2;

	double lg_gamma_low			= -1;
	double lg_gamma_high			=  3;
	double lg_gamma_inc		=  2;

	//struct parameters best_params;

	//TODO test that costs and gammas make sense
	//TODO DRY make fixture and constructor params from same variables
	ExperimentResultSet result_set;
	
	Options o;

	machine.parameter_search( 
		cross_val_exp, 
		f.input_set,
		o,
		lg_cost_low,
		lg_cost_high,
		lg_cost_inc,
		lg_gamma_low,
		lg_gamma_high,
		lg_gamma_inc,
		result_set
		);

	BOOST_CHECK_EQUAL( result_set.size(), 9 );//nested loop, n=3

	BOOST_CHECK_EQUAL( result_set.front()->lg_cost, lg_cost_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_cost, lg_cost_high );

	BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, lg_gamma_low );
	BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, lg_gamma_high );

	//check for correct values
	for( int result_i = 0; result_i < result_set.size(); result_i++ )
	{
		ExperimentResult* er = result_set.at(result_i);

		BOOST_CHECK_EQUAL( er->size(), f.input_set.size() );

		for( int i=0; i < f.input_set.size(); i++ )
		{	
			//TODO too much indirection here. law of demeter. too confusing to read
			BOOST_CHECK_EQUAL( er->at(i)->actual_susceptibility,
				f.input_set.at(i)->known_susceptibility() );

			//ok, I set threshold at 100000. That means every sequence is predicted to be susceptible
			// so my guess is all the svm's predictions will be susceptible as well.
			BOOST_CHECK_EQUAL( er->at(i)->predicted_susceptibility,
				true );
		}
	}

}

public:
	PreProcessor p;
	Options preproc_options;
	
	//input
	PreProcWUSet input_set;

};

//struct SvmMachineTest
//{
//	PreProcessor p;
//	Options preproc_options;
//	
//	//input
//	PreProcWUSet input_set;
//
//	//ctor
//	SvmMachineTest(){}
//
//	SvmMachineTest( std::string input_data_path, std::string drug )
//	{
//		p.parseInputFiles(
//		input_data_path,
//		"../data/shared/PI_wild.seq",
//		drug,
//		16,//threshold
//		preproc_options,
//		input_set
//		);		
//	}
//
//	SvmMachineTest( std::string input_data_path, std::string drug, double threshold )
//	{
//		p.parseInputFiles(
//		input_data_path,
//		"../data/shared/PI_wild.seq",
//		drug,
//		threshold,//threshold
//		preproc_options,
//		input_set
//		);	
//
//		Log::append( "SelfTestExperimentTest_Fixture input_set.size = " +  boost::lexical_cast<std::string>( input_set.size() )  );
//	}
//
//	//dtor
//	~SvmMachineTest(){}
//};

BOOST_AUTO_TEST_SUITE( SvmMachineTests_private );

//NFV median is 16 for pr 23
BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_23_threshold0 )
{
	SvmMachineTest t;
	t.pr_23_threshold0();	
}


BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_small23_threshold_10000 )
{
	SvmMachineTest t;
	t.pr_small23_threshold_10000();	
}

	//yep, I'm going to need an experiment result class.
	//or should the Experiment hold it's results and expose them?
	//nope a separate class b/c the experiment just runs a single experiment.
	//However, ExperimentResult and ExperimentResulset may be needed for all the parameter searches.


#if defined  LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_full_threshold0 )
{
	SvmMachineTest t;
	t.pr_full_threshold0();	
}

#endif //#if defined LONG_TESTS || TEST_ALL 

BOOST_AUTO_TEST_CASE( SvmMachineTest_pr_small23_threshold_10000_cross_val )
{
	SvmMachineTest t;
	t.pr_small23_threshold_10000_cross_val();
}


BOOST_AUTO_TEST_CASE( SvmMachineTest_run_model_validation )
{
		//hivm --operation test --drug NFV --thresholds 2 10 

	int ac = 17;
	char* av[ 17 ];
	av[0] = "hivm";
	av[1] = "--purpose";
	av[2] = "model-validation";
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


BOOST_AUTO_TEST_CASE( SvmMachineTest_run_memleak_check )
{
		//hivm --operation test --drug NFV --thresholds 2 10 

		int ac = 31;
		char* av[ 31 ];
		av[0] = "hivm";

		av[1] = "--lg-cost-low";
		av[2] = "1";
		av[3] = "--lg-cost-high";
		av[4] = "1";
		av[5] = "--lg-cost-inc";
		av[6] = "1";

		av[7] = "--lg-gamma-low";
		av[8] = "2";
		av[9] = "--lg-gamma-high";
		av[10] = "2";
		av[11] = "--lg-gamma-inc";
		av[12] = "1";

		av[13] = "--drug";
		av[14] = "NFV";

		av[17] = "--thresholds";//high
		av[18] = "10";

		av[15] = "--probability";//low, so out of order
		av[16] = "1";

		av[19] = "--wild-type";
		av[20] = "../data/shared/PI_wild.seq";

		av[21] = "--suscep-type";
		av[22] = "all";

		av[23] = "--seed";
		av[24] = "42";

		av[25] = "--output";
		av[26] = "../results/SvmMachineTest_run_memleak_check";

		av[27] = "--purpose";
		av[28] = "model-selection";

		av[29] = "--hivdb-file";
		av[30] = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV25.tsv";
				
		
		Options options(ac, av);

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}

//eyeball test to make sure it used entire dataset of 25
BOOST_AUTO_TEST_CASE( SvmMachineTest_run_model_selection_all_dataset )
{
		//hivm --operation test --drug NFV --thresholds 10 --all-dataset=1

		int ac = 33;
		char* av[ 33 ];
		av[0] = "hivm";

		av[1] = "--lg-cost-low";
		av[2] = "0";
		av[3] = "--lg-cost-high";
		av[4] = "1";
		av[5] = "--lg-cost-inc";
		av[6] = "1";

		av[7] = "--lg-gamma-low";
		av[8] = "2";
		av[9] = "--lg-gamma-high";
		av[10] = "4";
		av[11] = "--lg-gamma-inc";
		av[12] = "1";

		av[13] = "--drug";
		av[14] = "NFV";

		av[17] = "--thresholds";//high
		av[18] = "10";

		av[15] = "--probability";//low, so out of order
		av[16] = "1";

		av[19] = "--wild-type";
		av[20] = "../data/shared/PI_wild.seq";

		av[21] = "--suscep-type";
		av[22] = "all";

		av[23] = "--seed";
		av[24] = "42";

		av[25] = "--output";
		av[26] = "../results/SvmMachineTest_run_model_selection_all_dataset";

		av[27] = "--purpose";
		av[28] = "model-selection";

		av[29] = "--hivdb-file";
		av[30] = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV25.tsv";

		av[31] = "--all-dataset";
		av[32] = "1";
		
		Options options(ac, av);

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}

//eyeball test to make sure it used:
//A. entire 25 isolates in training set. (-f susceptibiltiy file)
//B. entire 6 isolates in test set. (-q --test-dataset)
BOOST_AUTO_TEST_CASE( SvmMachineTest_run_model_validation_separates_test_train_files )
{ 
		//hivm --operation test --drug NFV --thresholds 10 --all-dataset=1

		int ac = 35;
		char* av[ 35 ];
		av[0] = "hivm";

		av[1] = "--lg-cost-low";
		av[2] = "0";
		av[3] = "--lg-cost-high";
		av[4] = "1";
		av[5] = "--lg-cost-inc";
		av[6] = "1";

		av[7] = "--lg-gamma-low";
		av[8] = "2";
		av[9] = "--lg-gamma-high";
		av[10] = "4";
		av[11] = "--lg-gamma-inc";
		av[12] = "1";

		av[13] = "--drug";
		av[14] = "NFV";

		av[17] = "--thresholds";//high
		av[18] = "10";

		av[15] = "--probability";//low, so out of order
		av[16] = "1";

		av[19] = "--wild-type";
		av[20] = "../data/shared/PI_wild.seq";

		av[21] = "--suscep-type";
		av[22] = "all";

		av[23] = "--seed";
		av[24] = "42";

		av[25] = "--output";
		av[26] = "../results/SvmMachineTest_run_model_validation_separates_test_train_files";

		av[27] = "--purpose"; 
		av[28] = "model-validation";

		av[29] = "--hivdb-file";
		av[30] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv";

		av[31] = "--all-dataset";
		av[32] = "1";

		av[33] = "--test-dataset";
		av[34] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";

		Options options(ac, av);

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  SVMMACHINE_TEST || defined TEST_ALL
#endif//SVMMACHINETEST_HPP
