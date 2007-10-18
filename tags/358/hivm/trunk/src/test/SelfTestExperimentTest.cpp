#ifndef SELFTESTEXPERIMENTTEST_HPP
#define SELFTESTEXPERIMENTTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  SELFTESTEXPERIMENTTEST_TEST || defined TEST_ALL

#include "../SelfTestExperiment.hpp"
#include "../PreProcessor.hpp"
#include "../Log.hpp"
#include "../ExperimentResult.hpp"
#include "../ExperimentAnalyzer.hpp"//has caculate accuracy

#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct SelfTestExperimentTest_Fixture
{
	PreProcessor p;
	Options preproc_options;

	//input
	PreProcWUSet input_set;

	//ctor
	SelfTestExperimentTest_Fixture(){}

	SelfTestExperimentTest_Fixture( std::string input_data_path, std::string drug )
	{
		p.parseInputFiles(
		input_data_path,
		"../data/shared/PI_wild.seq",
		drug,
		16,//threshold
		preproc_options,
		input_set
		);	

		Log::append( "SelfTestExperimentTest_Fixture input_set.size = " +  boost::lexical_cast<std::string>( input_set.size() )  );
	}

	SelfTestExperimentTest_Fixture( std::string input_data_path, std::string drug, double threshold )
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
	~SelfTestExperimentTest_Fixture(){}
};

//BOOST_AUTO_TEST_SUITE( ExperimentResultTests );
//
//BOOST_AUTO_TEST_CASE( ExperimentResult_constructor_destructor )
//{
//	{
//		SelfTestExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
//	
//		ExperimentResult *result = new ExperimentResult( -13, 3 );
//	
//		//struct ExperimentResult::data_point* point = new ExperimentResult::data_point();
//		struct ExperimentResult::data_point* point = new ExperimentResult::data_point( true, false );
//		result->data.data_points.push_back( point );
//
//		BOOST_CHECK_EQUAL( result->log_cost, -13 );
//		BOOST_CHECK_EQUAL( result->log_gamma, 3 );
//		BOOST_CHECK_EQUAL( result->data.data_points.front()->predicted_susceptibility, true );
//		BOOST_CHECK_EQUAL( result->data.data_points.front()->actual_susceptibility, false );
//	
//		delete result;
//	}
//}
//
//BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE( SelfTestExperimentTests );

//TODO possible unit test.
// take 2 different sample sizes of tests, find a c,g that is 1 and make sure that it is with 5% on the other sample size.
//difficult to full automate this. Maybe just pick out values like this by hand from my tests, and make sure 
//that for the 25 row (quick), that is keeps running the same accuracy?

BOOST_AUTO_TEST_CASE( SelfTestExperimentTest_run_experiment_pr_small23 )
{
	SelfTestExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
	SelfTestExperiment ste;
	//ExperimentResult *er = new ExperimentResult( 2, 2);

	Options o;
	o.log_cost = 2;
	o.log_gamma = 2;

	ExperimentResultSet results;

	ste.run_experiment( f.input_set, results, o);

	//check out ExperimentResult more here.

	BOOST_CHECK_EQUAL( results.front()->log_cost,  2 );
	BOOST_CHECK_EQUAL( results.front()->log_gamma, 2  );

	BOOST_CHECK_EQUAL( results.front()->data.data_points.size(), f.input_set.size() );

	//check to make sure all actual predictions recorded correctly
	for( size_t i=0; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( results.front()->data.data_points.at(i)->actual_susceptibility, f.input_set.at(i)->known_susceptibility() );
	}
	
	//delete er;

	//OK, let's test for predicted as well.
	//one way is to crank up the threshold to 10,000 or down to 0.
	//That should make all predictions on one side.


	//std::string msg = "SelfTestExperimentTest_run_experiment_pr_small23 accuracy: ";
	//msg.append( boost::lexical_cast<std::string>( accuracy ) );
	//Log::append( msg );
	//std::cout << msg;

	//test. well, not sure how to test this.
	//automatically checks for memory leaks at least.
}

BOOST_AUTO_TEST_CASE( SelfTestExperimentTest_run_experiment_pr_small23_threshold_0 )
{
	SelfTestExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV", 0);
	SelfTestExperiment ste;
	//ExperimentResult *er = new ExperimentResult( 1, 1);
	ExperimentResultSet results;

	Options o;
	o.log_cost = 1;
	o.log_gamma = 1;

	ste.run_experiment( f.input_set, results, o );

	BOOST_CHECK_EQUAL( results.front()->data.data_points.size(), f.input_set.size() );

	//ok, I set threshold at 0. That means every sequence is predicted to be not_susceptible
	// so my guess is all the svm's predictions will be not_susceptible as well.
	for( size_t i=0; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( results.front()->data.data_points.at(i)->predicted_susceptibility,
			false );
	}
	
	//delete er;
}

BOOST_AUTO_TEST_CASE( SelfTestExperimentTest_run_experiment_pr_small23_threshold_10000 )
{
	SelfTestExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv",
		"NFV", 10000);
	SelfTestExperiment ste;
	//ExperimentResult *er = new ExperimentResult( 1, 1);
	ExperimentResultSet results;

	Options o;
	o.log_cost = 1;
	o.log_gamma = 1;

	ste.run_experiment( f.input_set, results, o );

	BOOST_CHECK_EQUAL( results.front()->data.data_points.size(), f.input_set.size() );

	//ok, I set threshold at 100000. That means every sequence is predicted to be susceptible
	// so my guess is all the svm's predictions will be susceptible as well.
	for( size_t i=0; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( results.front()->data.data_points.at(i)->predicted_susceptibility,
			true );
	}
	
	//delete er;
}


//This test isn't very useful since refactored out accuracy.
//BOOST_AUTO_TEST_CASE( SelfTestExperimentTest_run_experiment_pr_small23_cost_gamma )
//{
//	SelfTestExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
//	SelfTestExperiment ste;
//	
//	double log_cost = -1;
//	double log_gamma = -1;
//	ExperimentResult *er = new ExperimentResult( log_cost, log_gamma );
//
//	ste.run_experiment( f.input_set, er, log_cost, log_gamma );
//
//	//std::string msg = "SelfTestExperimentTest_run_experiment_pr_small23 accuracy: ";
//	//msg.append( boost::lexical_cast<std::string>( accuracy ) );
//	//msg.append( "log_cost = 30, log_gamma = -2" );
//	//Log::append( msg );
//	//std::cout << msg;
//
//	//test gamma and cost are correctly set in different experiments
//	ste.run_experiment( f.input_set, er );
//
//	delete er;
//
////	BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (accuracy) (accuracy_default) );
//
//}

#if defined  LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( SelfTestExperimentTest_pr_NFV25_t10 )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file

	//initialize
	SelfTestExperimentTest_Fixture f( 
		test_data_path, 
		drug,
		threshold );	

	{
		SelfTestExperiment ste;

		////Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		double log_cost = 3;
		double log_gamma = -5;
		//ExperimentResult *er = new ExperimentResult( log_cost, log_gamma );
		
		Options o;
		o.log_cost = log_cost;
		o.log_gamma = log_gamma;
		
		ExperimentResultSet results;

		ste.run_experiment( f.input_set, results, o );

		//analyze
		ExperimentAnalyzer ea;
		double test_accuracy = ea.accuracy( results.front() );
		double previous_accuracy = .92;//Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		
		BOOST_CHECK_EQUAL( test_accuracy, previous_accuracy );
		
		//delete er;
	}

	{
		SelfTestExperiment ste;

		////Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		double log_cost = 1;
		double log_gamma = -1;
		
		//ExperimentResult *er = new ExperimentResult( log_cost, log_gamma );
		
		Options o;
		o.log_cost = log_cost;
		o.log_gamma = log_gamma;
	
		ExperimentResultSet results;

		ste.run_experiment( f.input_set, results, o );

		//analyze
		ExperimentAnalyzer ea;
		double test_accuracy = ea.accuracy( results.front() );
		double previous_accuracy = 1;//Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		
		BOOST_CHECK_EQUAL( test_accuracy, previous_accuracy );
		
		//delete er;
	}

	{
		SelfTestExperiment ste;

		////Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		double log_cost = -5;
		double log_gamma = 3;
		
		//ExperimentResult *er = new ExperimentResult( log_cost, log_gamma );
		
		Options o; 
		o.log_cost = log_cost;
		o.log_gamma = log_gamma;

		ExperimentResultSet results;
		ste.run_experiment( f.input_set, results, o );

		//analyze
		ExperimentAnalyzer ea;
		double test_accuracy = ea.accuracy( results.front() );
		double previous_accuracy = 0.56;//Fri_Jun_16_03-08-21_2006_SelfTest_NFV_t10_seqs25.csv
		
		BOOST_CHECK_EQUAL( test_accuracy, previous_accuracy );
		
		//delete er;
	}
}

#endif

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  SELFTESTEXPERIMENTTEST_TEST || defined TEST_ALL
#endif//SELFTESTEXPERIMENTTEST_HPP
