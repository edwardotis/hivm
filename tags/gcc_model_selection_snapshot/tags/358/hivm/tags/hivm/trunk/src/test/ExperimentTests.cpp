//@purpose before I create the cmd line parser,
//this is where I'll store experiments to run
//not really a unit test here.
#ifndef EXPERIMENTTESTS_HPP
#define EXPERIMENTTESTS_HPP

#include "Definitions.hpp"
#if defined  EXPERIMENTTESTS_TEST

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/lexical_cast.hpp> //safe numerical to string casts


#include "../ExperimentAnalyzer.hpp"
#include "../ExperimentResult.hpp"
#include "../PreProcessor.hpp"
#include "../SelfTestExperiment.hpp"
#include "../SvmMachine.hpp"
#include "../Log.hpp"
#include "../FunctionObjects.cpp"//functor for random integer in random_shuffle
#include "../CrossValidationExperiment.hpp"
#include "../ValidationExperiment.hpp"
#include "../Options.hpp"

struct ExperimentTests_Fixture
{
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	SvmMachine machine;
	SelfTestExperiment self_test_exp;
	CrossValidationExperiment cross_val_exp;
	ExperimentResultSet result_set;
	Options options;

	//variables grid search
	double log_cost_low;
	double log_cost_high;
	double log_cost_increment;

	double log_gamma_low;
	double log_gamma_high;
	double log_gamma_increment;

	//ctor cross val
	ExperimentTests_Fixture(
		std::string test_data_path,
		std::string wild_seq, 
		std::string drug, 
		std::vector<double> thresholds,
		size_t seed,
		std::string experiment_type )
	{
		p.parseInputFiles(
		test_data_path,
		wild_seq,
		drug,
		thresholds,//threshold
		options,
		seed,
		input_train_set,
		input_test_set
		);
	
		//setup result set
		result_set.set_seed( seed );

		result_set.set_params(
			experiment_type,
			drug,
			thresholds.at(0),
			wild_seq,
			test_data_path,
			input_train_set.size()
			);

	log_cost_low			=  3.5;
	log_cost_high			=  6.5;
	log_cost_increment		=  1;

	log_gamma_low			= -5.5;
	log_gamma_high			= -2.5;
	log_gamma_increment		=  1;

	Log::append( "ExperimentTests_Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	}

	//ctor cross val
	ExperimentTests_Fixture(
		std::string test_data_path,
		std::string wild_seq, 
		std::string drug, 
		double threshold,
		size_t seed,
		std::string experiment_type )
	{
		p.parseInputFiles(
		test_data_path,
		wild_seq,
		drug,
		threshold,//threshold
		options, 
		seed,
		input_train_set,
		input_test_set
		);
	
		//setup result set
		result_set.set_seed( seed );

		result_set.set_params(
			experiment_type,
			drug,
			threshold,
			wild_seq,
			test_data_path,
			input_train_set.size()
			);

	log_cost_low			= -5;
	log_cost_high			=  5;
	log_cost_increment		=  2;

	log_gamma_low			= -5;
	log_gamma_high			=  5;
	log_gamma_increment		=  2;

	Log::append( "ExperimentTests_Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	}

	//ctor SelfTest
	ExperimentTests_Fixture(std::string test_data_path, std::string wild_seq, std::string drug, double threshold,
		std::string experiment_type,
		bool is_self_test )
	{
		p.parseInputFiles(
		test_data_path,
		wild_seq,
		drug,
		threshold,//threshold
		options, 
		input_train_set
		);
	
		//setup result set
		result_set.set_params(
			experiment_type,
			drug,
			threshold,
			wild_seq,
			test_data_path,
			input_train_set.size()
			);

	log_cost_low			= -5;
	log_cost_high			=  5;
	log_cost_increment		=  2;

	log_gamma_low			= -5;
	log_gamma_high			=  5;
	log_gamma_increment		=  2;

	Log::append( "ExperimentTests__Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	}

		//dtor
	~ExperimentTests_Fixture(){}
};

BOOST_AUTO_TEST_SUITE( ExperimentTests );



#if defined  LONG_TESTS

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFVall_t2_t10_Test_single_cg_pair_val_pred )
//{
//	//ok, goal is to take create a training model with a c,g pair that I give here.
//	//then predict on the test data set
//	//and record the accuracy
//	
//	//set variables
//	std::string test_data_path = "../data/shared/PR_2006-05-25_v2.0.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	//double threshold = 10;//should be median of the entire file
//	int seed = 42;
//	Options options;
//
//	std::vector<double> thresholds;
//	thresholds.push_back( 2 );
//	thresholds.push_back( 10 );
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path,
//		wild_seq_path,
//		drug,
//		thresholds,
//		seed,
//		"Validate_CrossValidation"
//		);	
//
//	ValidationExperiment ve;
//	//we'll create a Predict function or class later, if we need it.
//
//	//run experiment on BEST c,g
//	double best_log_cost  =  6.5;//chosen from best cross val results 
//	double best_log_gamma = -5.5;//chosen from best cross val result
//
//	ExperimentResult* er_best = new ExperimentResult(
//		best_log_cost, best_log_gamma
//		);
//
//	ve.run_experiment( 
//		f.input_train_set,
//		f.input_test_set,
//		options,
//		best_log_cost,
//		best_log_gamma,
//		er_best
//		);
//
//	f.result_set.push_back( er_best );
//
//	//run experiment on WORST c,g
//	//double worst_log_cost  = -3;//chosen from best cross val results 
//	//double worst_log_gamma = -5;//chosen from best cross val result
//
//	//ExperimentResult* er_worst = new ExperimentResult(
//	//	worst_log_cost, worst_log_gamma
//	//	);
//
//	//ve.run_experiment( 
//	//	f.input_train_set,
//	//	f.input_test_set,
//	//	worst_log_cost,
//	//	worst_log_gamma,
//	//	er_worst
//	//	);
//
//	//f.result_set.push_back( er_worst );
//
////	BOOST_CHECK_EQUAL( f.result_set.size(), 2 );
//	//BOOST_CHECK_EQUAL( f.result_set.front()->log_cost, 5 );
//	//BOOST_CHECK_EQUAL( f.result_set.front()->log_gamma, -5 );
//	//BOOST_CHECK_EQUAL( f.result_set.back()->log_cost, -3 );
//	//BOOST_CHECK_EQUAL( f.result_set.back()->log_gamma, -5 );
//
//	BOOST_CHECK_EQUAL( f.result_set.experiment_type, "Validate_CrossValidation" );
//
//	//analyze best and worst
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//
//	//BOOST_CHECK_EQUAL( f.result_set.size(), 2 );
//	//BOOST_CHECK_EQUAL( f.result_set.front()->log_cost, 5 );
//	//BOOST_CHECK_EQUAL( f.result_set.front()->log_gamma, -5 );
//	//BOOST_CHECK_EQUAL( f.result_set.back()->log_cost, -3 );
//	//BOOST_CHECK_EQUAL( f.result_set.back()->log_gamma, -5 );
//
//	//delete er_best;
//	//delete er_worst;
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFVall_t10_crossval )
//{
//	//set variables
//	std::string test_data_path = "../data/shared/PR_2006-05-25_v2.0.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the entire file
//	int seed = 42;
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold,
//		seed,
//		"Cross_Validation_Experiment"
//		);	
//
////Rerun of just one pair.
//double log_cost_low				=   5;
//double log_cost_high			=   5;
//double log_cost_increment		=  .5;
//
//double log_gamma_low			=  -5;
//double log_gamma_high			=  -5;
//double log_gamma_increment		=  .5;
//
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.cross_val_exp,
//		f.input_train_set,
//		log_cost_low,
//		log_cost_high,
//		log_cost_increment,
//		log_gamma_low,
//		log_gamma_high,
//		log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//
//	//tests?
//	//Let's just see the output first, then we can think on this more. :)
//}

#endif //#if defined  LONG_TESTS || defined TEST_ALL

#include <algorithm>//shuffle
BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV50_t10_random_shuffle )
{
	//set variables
	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	int seed = 42;
	Options options;

	//initialize
	ExperimentTests_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Self_Test_Experiment",
		true
		 );

	//just testing whether shuffle is randomly seeded here.

	//make 2nd independent copy of input data set
	PreProcWUSet input_set2;
	//PreProcWUSet input_test_set2;
	PreProcessor p2;
	p2.parseInputFiles(
		test_data_path,
		wild_seq_path,
		drug,
		threshold,//threshold
		options, 
		input_set2
		);
	
	
	//front's should be the same id
	BOOST_CHECK_EQUAL( input_set2.front()->get_id(), f.input_train_set.front()->get_id() );

	//shuffle set 1
	std::random_shuffle( f.input_train_set.begin(), f.input_train_set.end() );

	//front's should be different id's
	BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(),
	( input_set2.front()->get_id() ) ( f.input_train_set.front()->get_id() ) );

	//shuffle set 2
	std::random_shuffle( input_set2.begin(), input_set2.end() );

	//front's should still be different id's if shuffle is truly random each time
	BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(),
	( input_set2.front()->get_id() ) ( f.input_train_set.front()->get_id() ) );

	//now check for whether
}



//TODO test this on gcc, to see if the srand48 'gotcha' is in effect
BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV50_t10_random_shuffle_srand )
{
	//set variables
	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	size_t seed = 42;
	Options options;
	
	//initialize
	ExperimentTests_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Self_Test_Experiment",
		true
		);

	//just testing whether shuffle is randomly seeded here.

	//make 2nd independent copy of input data set
	PreProcWUSet input_set2;
	PreProcessor p2;
	p2.parseInputFiles(
		test_data_path,
		wild_seq_path,
		drug,
		threshold,//threshold
		options, 
		input_set2
		);


	//SEED MUST BE SET BEFORE EACH CALL TO Rand!
	
	//front's should be the same id
	BOOST_CHECK_EQUAL( input_set2.front()->get_id(), f.input_train_set.front()->get_id() );

	RandInt Rand( 42 );
	
	//shuffle set 1 w/ seed 42
	//srand(42);//so seeding it here.
	std::random_shuffle( f.input_train_set.begin(), f.input_train_set.end(), Rand );

	//front's should be different id's
	BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(),
	( input_set2.front()->get_id() ) ( f.input_train_set.front()->get_id() ) );

	//shuffle set 2 w/ seed 42
	//srand(42);//so seeding it here.
	RandInt Rand2( 42 );
	std::random_shuffle( input_set2.begin(), input_set2.end(), Rand2 );

	//front's should now be same again since I used the same seed for shuffling and restarted the 
	//random number generator
	BOOST_CHECK_EQUAL( input_set2.front()->get_id(), f.input_train_set.front()->get_id() );

	//shuffle set1 second time w/ seed 42
	//srand(42);//so seeding it here.
	std::random_shuffle( f.input_train_set.begin(), f.input_train_set.end(), Rand );

	//front's should be different id's
	BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(),
	( input_set2.front()->get_id() ) ( f.input_train_set.front()->get_id() ) );

	//shuffle set 2 second time w/ seed 42
	//srand(42);//so seeding it here.
	std::random_shuffle( input_set2.begin(), input_set2.end(), Rand2 );

	//front's should now be same again since I used the same seed for shuffling both times
	BOOST_CHECK_EQUAL( input_set2.front()->get_id(), f.input_train_set.front()->get_id() );

}


//SELF TESTS BELOW HERE. ALL BUT ONE OF SIZE 400 ARE UNNECESSARY TESTS.

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV400_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV400.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//	bool self_test = true;
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold,
//		"Self_Test_Experiment",
//		self_test
//			);	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_train_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}
//
//



//BOOST_AUTO_TEST_CASE( ExperimentTest_prAll_NFV_t2_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/shared/PR_2006-05-25_v2.0.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	//double threshold = 10;//should be median of the file
//	bool self_test = true;
//	size_t seed = 42;
//
//	std::vector<double> thresholds;
//	thresholds.push_back( 2 );
//	thresholds.push_back( 10 );
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		thresholds,
//		seed,
//		"Cross_Val_Experiment"
//			);	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.cross_val_exp,
//		f.input_train_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}
//
//



//Shuffle
//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV50_t10_shuffle )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	std::random_shuffle( f.input_set.begin(), f.input_set.end() );
//    
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_prsmall3_NFV_t2 )
//{
//	//set variables
//	//std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV400.tsv";
//	std::string test_data_path = "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 2;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//variables grid search
//	//TODO make this a struct?
//	double log_cost_low			= -1;
//	double log_cost_high			=  3;
//	double log_cost_increment		=  2;
//
//	double log_gamma_low			= -1;
//	double log_gamma_high			=  3;
//	double log_gamma_increment		=  2;
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		log_cost_low,
//		log_cost_high,
//		log_cost_increment,
//		log_gamma_low,
//		log_gamma_high,
//		log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//
//	//tests?
//	//check by hand
//	//read them back in from file?
//
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV25_t10_16to16 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV25.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		-16,
//		16,
//		f.log_cost_increment,
//		-16,
//		16,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

//Bigger c,g space
//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV25_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV25.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV50_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV100_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV100.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

//BOOST_AUTO_TEST_CASE( ExperimentTest_pr_NFV200_t10 )
//{
//	//set variables
//	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV200.tsv";
//	std::string wild_seq_path = "../data/shared/PI_wild.seq";
//	std::string drug = "NFV";
//	double threshold = 10;//should be median of the file
//
//	//initialize
//	ExperimentTests_Fixture f( 
//		test_data_path, 
//		wild_seq_path,
//		drug,
//		threshold );	
//
//	//run experiment through grid search
//	f.machine.parameter_search(
//		f.self_test_exp,
//		f.input_set,
//		f.log_cost_low,
//		f.log_cost_high,
//		f.log_cost_increment,
//		f.log_gamma_low,
//		f.log_gamma_high,
//		f.log_gamma_increment,
//		f.result_set
//		);
//
//	//analyze
//	ExperimentAnalyzer ea;
//	ea.analyze( f.result_set );
//}

BOOST_AUTO_TEST_SUITE_END();

//// return an integral random number in the range 0 - (n - 1)
//int Rand(int n)
//{
//    return rand() % n ;
//}
//
//struct RandInt
//{
//	int operator() (int m)
//	{
//		return rand() % m; 
//	}
//
//} rand_int; 

#endif // #if defined  EXPERIMENTTESTS_TEST || defined TEST_ALL
#endif//EXPERIMENTTESTS_HPP
