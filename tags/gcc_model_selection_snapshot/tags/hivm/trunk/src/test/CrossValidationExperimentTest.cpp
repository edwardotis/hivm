#ifndef CROSSVALIDATIONEXPERIMENTTEST_HPP
#define CROSSVALIDATIONEXPERIMENTTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"

#include "../PreProcessor.hpp"
#include "../Log.hpp"
#include "../ExperimentResult.hpp"
#include "../Options.hpp"

#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

#if defined  CROSSVALIDATIONEXPERIMENTTEST_TEST || defined TEST_ALL

#include "../CrossValidationExperiment.hpp"

struct CrossValidationExperimentTest_Fixture
{
	PreProcessor p;
	PreProcWUSet input_set;
	ExperimentResultSet result_set;
	Options o;

	//ctor
	CrossValidationExperimentTest_Fixture(std::string test_data_path, std::string wild_seq, std::string drug, double threshold)
	{
	
		p.parseInputFiles(
		test_data_path,
		wild_seq,
		drug,
		threshold,
		o,
		input_set
		);

		//setup result set
		result_set.set_params(
			"SelfTest",
			drug,
			threshold,
			wild_seq,
			test_data_path,
			input_set.size()
			);

	Log::append( "CrossValidationExperimentTest_Fixture input_set.size = " +  boost::lexical_cast<std::string>( input_set.size() )  );
	
	}

	CrossValidationExperimentTest_Fixture( std::string input_data_path, std::string drug )
	{
		p.parseInputFiles(
		input_data_path,
		"../data/shared/PI_wild.seq",
		drug,
		16,//threshold
		o,
		input_set
		);	

		Log::append( "CrossValidationExperimentTest_Fixture input_set.size = " +  boost::lexical_cast<std::string>( input_set.size() )  );
	}

	//dtor
	~CrossValidationExperimentTest_Fixture(){}
};

BOOST_AUTO_TEST_SUITE( CrossValidationExperimentTests );

BOOST_AUTO_TEST_CASE( CrossValidationExperimentTest_create_fold_pr_small23_NFV_fold0 )
{
	CrossValidationExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
	CrossValidationExperiment cve;

	//output
	SvmWUMatrix trainset_output;
	SvmWUMatrix predictees_output;

	cve.createFold( f.input_set, 0, trainset_output, predictees_output );

	//sizes
	size_t total_size = trainset_output.size() + predictees_output.size();
	BOOST_CHECK_EQUAL( f.input_set.size(), total_size );
	BOOST_CHECK_EQUAL( trainset_output.size(), 20 );
	BOOST_CHECK_EQUAL( predictees_output.size(), 3 );
		
	////tests
	////correct seqs in input and output based on fold, checked by IDs
	BOOST_CHECK_EQUAL( "CA3809", predictees_output.at(0)->get_id() );
	BOOST_CHECK_EQUAL( "CA3872", predictees_output.at(1)->get_id() );
	BOOST_CHECK_EQUAL( "SD-7",   predictees_output.at(2)->get_id() );

	//test predictees
	BOOST_CHECK_EQUAL( f.input_set.at(0)->get_id(), predictees_output.at(0)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(1)->get_id(), predictees_output.at(1)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(2)->get_id(), predictees_output.at(2)->get_id() );

	BOOST_CHECK_EQUAL( f.input_set.at(0)->known_susceptibility(), predictees_output.at(0)->known_susceptibility() );
	BOOST_CHECK_EQUAL( f.input_set.at(1)->known_susceptibility(), predictees_output.at(1)->known_susceptibility() );
	BOOST_CHECK_EQUAL( f.input_set.at(2)->known_susceptibility(), predictees_output.at(2)->known_susceptibility() );

	//test trainset
	int remainder = 3;
	for( size_t i = 0; i < f.input_set.size()-remainder; i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i+remainder)->get_id(), trainset_output.at(i)->get_id() );
	}

	//23 seqs for mod test
	//fewer than 10 sequences - warn user and exit
	//full set - 

	//folds 0, 4, 5, 9, 10	
}

BOOST_AUTO_TEST_CASE( CrossValidationExperimentTest_create_fold_pr_small23_NFV_fold9 )
{
	CrossValidationExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
	CrossValidationExperiment cve;

	//output
	SvmWUMatrix trainset_output;
	SvmWUMatrix predictees_output;

	cve.createFold( f.input_set, 9, trainset_output, predictees_output );

	//sizes
	size_t total_size = trainset_output.size() + predictees_output.size();
	BOOST_CHECK_EQUAL( f.input_set.size(), total_size );
	BOOST_CHECK_EQUAL( trainset_output.size(), 21 );
	BOOST_CHECK_EQUAL( predictees_output.size(), 2 );
		
	//tests
	//correct seqs in input and output based on fold, checked by IDs

	//test predictees
	//BOOST_CHECK_EQUAL( f.input_set.at(20)->get_id(), predictees_output.at(20)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(21)->get_id(), predictees_output.at(0)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(22)->get_id(), predictees_output.at(1)->get_id() );

	//test trainset
	for( size_t i = 0; i < 21; i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->get_id(), trainset_output.at(i)->get_id() );
	}
}

BOOST_AUTO_TEST_CASE( CrossValidationExperimentTest_create_fold_pr_small23_NFV_fold3 )
{
	CrossValidationExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
	CrossValidationExperiment cve;

	//output
	SvmWUMatrix trainset_output;
	SvmWUMatrix predictees_output;

	cve.createFold( f.input_set, 3, trainset_output, predictees_output );

	//sizes
	size_t total_size = trainset_output.size() + predictees_output.size();
	BOOST_CHECK_EQUAL( f.input_set.size(), total_size );
	BOOST_CHECK_EQUAL( trainset_output.size(), 21 );
	BOOST_CHECK_EQUAL( predictees_output.size(), 2 );
		
	//tests
	//correct seqs in input and output based on fold, checked by IDs

	//test predictees
	//BOOST_CHECK_EQUAL( f.input_set.at(20)->get_id(), predictees_output.at(20)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(9)->get_id(), predictees_output.at(0)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(10)->get_id(), predictees_output.at(1)->get_id() );

	//test trainset
	for( size_t i = 0; i < 9; i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->get_id(), trainset_output.at(i)->get_id() );
	}

	for( size_t i = 11; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->get_id(), trainset_output.at(i-2)->get_id() );
	}
}

BOOST_AUTO_TEST_CASE( CrossValidationExperimentTest_create_fold_pr_small23_NFV_fold2 )
{
	CrossValidationExperimentTest_Fixture f( "../data/CrossValidationExperimentTest/PR_2006-05-25_v2.0_small23.tsv", "NFV");
	CrossValidationExperiment cve;

	//output
	SvmWUMatrix trainset_output;
	SvmWUMatrix predictees_output;

	cve.createFold( f.input_set, 2, trainset_output, predictees_output );

	//sizes
	size_t total_size = trainset_output.size() + predictees_output.size();
	BOOST_CHECK_EQUAL( f.input_set.size(), total_size );
	BOOST_CHECK_EQUAL( trainset_output.size(), 20 );
	BOOST_CHECK_EQUAL( predictees_output.size(), 3 );
		
	//tests
	//correct seqs in input and output based on fold, checked by IDs

	//test predictees
	BOOST_CHECK_EQUAL( f.input_set.at(6)->get_id(), predictees_output.at(0)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(7)->get_id(), predictees_output.at(1)->get_id() );
	BOOST_CHECK_EQUAL( f.input_set.at(8)->get_id(), predictees_output.at(2)->get_id() );

	//test trainset
	for( size_t i = 0; i < 6; i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->get_id(), trainset_output.at(i)->get_id() );
	}

	for( size_t i = 9; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->get_id(), trainset_output.at(i-3)->get_id() );
	}
}

//ok, folds seem to be solid.
//now we just put it together
BOOST_AUTO_TEST_CASE( CrossValidationExperimentTest_pr50_NFV_single_cg_pair )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV50.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file

	//initialize
	CrossValidationExperimentTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold 
		);	

	double log_cost  = 1;
	double log_gamma = 1;

	ExperimentResult* er = new ExperimentResult(
		log_cost, log_gamma
		);

	//cross validation single c,g
	//later when running parameter search, make sure
	//this single c,g has same results in the param_search
	Options o;
	CrossValidationExperiment cve;
	cve.run_experiment(f.input_set, er, o, log_cost, log_gamma);

	//Tests?
	// # of data points.
	int num_data_points = 50;// 5 predictions in 10 iterations (folds)
	BOOST_CHECK_EQUAL( num_data_points, er->datum.data_points.size() );
	
	//actual suscep in data points == input_set known_susceptibility
	for( size_t i=0; i < f.input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( f.input_set.at(i)->known_susceptibility(),
			er->datum.data_points.at(i)->actual_susceptibility );
	}
	
	delete er;
}

BOOST_AUTO_TEST_SUITE_END();

#endif // CROSSVALIDATIONEXPERIMENTTEST_TEST || defined TEST_ALL

#if defined  VALIDATIONEXPERIMENTTEST_TEST || defined TEST_ALL

#include "../ValidationExperiment.hpp"

struct ValidationExperimentTest_Fixture
{
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	ExperimentResultSet result_set;
	Options options;

	//ctor
	ValidationExperimentTest_Fixture(
		std::string test_data_path, 
		std::string wild_seq, 
		std::string drug, 
		double threshold,
		size_t seed
		)
	{
	
		p.parseInputFiles(
		test_data_path,
		wild_seq,
		drug,
		threshold,
		options,
		seed,
		input_train_set,
		input_test_set
		);

		//setup result set
		result_set.set_params(
			"SelfTest",
			drug,
			threshold,
			wild_seq,
			test_data_path,
			input_train_set.size()
			);

	Log::append( "ValidationExperimentTest_Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	
	}//end ctor
};


BOOST_AUTO_TEST_SUITE( ValidationExperimentTests );

BOOST_AUTO_TEST_CASE( ValidationExperimentTest_pr50_NFV_single_cg_pair )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV50.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	size_t seed = 42;//seed for randomly splitting test and training data sets

	//initialize
	ValidationExperimentTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		seed
		);	

	double log_cost  = 1;
	double log_gamma = 1;

	ExperimentResult* er = new ExperimentResult(
		log_cost, log_gamma
		);

	//cross validation single c,g
	//later when running parameter search, make sure
	//this single c,g has same results in the param_search
	Options o;
	ValidationExperiment ve;
	ve.run_experiment(
		f.input_train_set,
		f.input_test_set,
		o,
		log_cost, 
		log_gamma,
		er);

	//Tests
	BOOST_CHECK_EQUAL( er->log_cost, 1 );
	BOOST_CHECK_EQUAL( er->log_gamma, 1 );

	// # of data points.
	int num_data_points = 20;// from log after cleaning and splitting
	BOOST_CHECK_EQUAL( num_data_points, er->datum.data_points.size() );
	
	//actual suscep in data points == input_set known_susceptibility
	for( size_t i=0; i < f.input_test_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( f.input_test_set.at(i)->known_susceptibility(),
			er->datum.data_points.at(i)->actual_susceptibility );
	}
	
	delete er;
}

BOOST_AUTO_TEST_SUITE_END();

#endif // #if defined  VALIDATIONEXPERIMENTTEST_TEST || defined TEST_ALL
#endif//CROSSVALIDATIONEXPERIMENTTEST_HPP
