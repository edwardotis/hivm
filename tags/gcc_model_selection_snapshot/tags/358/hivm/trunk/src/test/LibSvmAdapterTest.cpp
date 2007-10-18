#ifndef LIBSVMADAPTERTEST_HPP
#define LIBSVMADAPTERTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  LIBSVMADAPTER_TEST || defined TEST_ALL

#include "../LibSvmAdapter.hpp"
#include "../PreProcessor.hpp"
#include "../PreProcWorkUnit.hpp"
#include "../BioProcessor.hpp"
#include "../Log.hpp"
#include "../Options.hpp"
#include "../ExperimentResult.hpp"//for prediction results
//#include "../svm.h"

#include <boost/lexical_cast.hpp> //safe numerical to string casts
//#include "../LibSvmFacade.h"
//#include "../svm.cpp"//svm_model defined here, not in svm.h

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values


struct LibSvmAdapterTest_Fixture
{
	//vars
	PreProcessor p;
	PreProcWUSet wu_set;

	PreProcWUSet predictee_set;

	BioProcessor bio_proc;

	Options preproc_options;

	//OUTPUT
	SvmWUMatrix svm_train_matrix;  //size 3x3 if small3.tsv
	SvmWUMatrix svm_predictee_matrix;//size 2x3 if small3.tsv

	//ctor
    LibSvmAdapterTest_Fixture( )
	{}

	//ctor
	LibSvmAdapterTest_Fixture( std::string input_data_path )
	{	
		p.parseInputFiles(
			input_data_path,
			"../data/shared/PI_wild.seq",
			"NFV",//NFV is standard for unit tests here.
			2,
			preproc_options,
			wu_set
			);	

		Log::append( "LibSvmAdapterTest_Fixture wu_set.size = " +  boost::lexical_cast<std::string>( wu_set.size() )  );


		if( wu_set.size() > 3 )
		{
			//add 2 wu's to it from the end of the inputset
			predictee_set.push_back( wu_set.back() );//SD-7 if small3.tsv
			wu_set.pop_back();
			predictee_set.push_back( wu_set.back() );//CA3876 if small3.tsv
			wu_set.pop_back();

			//bio_proc.process( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
			bio_proc.process_with_cache( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
		}
		else
		{
			BOOST_FAIL( "Less than 4 sequences loaded intoLibSvmAdapterTest_Fixture. Test won't work." );
		}
			

	}

	//dtor
    ~LibSvmAdapterTest_Fixture(){}


};

BOOST_AUTO_TEST_SUITE( LibSvmAdapterTests );

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_problem_categories )
{
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	
	LibSvmAdapter l;
	double* testee;
	testee = l.create_svm_problem_categories( f.svm_train_matrix );

	//test for 1 and -1 in correct places
	//test lenth

	//NFV threshold 2
	BOOST_CHECK_EQUAL( -1, *testee );//2.2
	BOOST_CHECK_EQUAL( 1, *(testee+1) );//.8
	BOOST_CHECK_EQUAL( 1, *(testee+2) );//1.2

	free( testee );
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_node_ptr )
{
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	
	struct svm_node* node;

	LibSvmAdapter l;
	node = l.create_svm_node_ptr( f.svm_train_matrix.front() );

	SvmWUSet* s = f.svm_train_matrix.front();

	//test values. there are 3
	BOOST_CHECK_EQUAL( node->index,   0  );
	BOOST_CHECK_EQUAL( node->value,   s->front()  );// 2.2

	BOOST_CHECK_EQUAL( (node+1)->index,   1  );
	BOOST_CHECK_EQUAL( (node+1)->value, s->at(1) );//  .8

	BOOST_CHECK_EQUAL( (node+2)->index,   2  );
	BOOST_CHECK_EQUAL( (node+2)->value, s->at(2) );// 1.2

	//test special libsvm termination symbol for node ptrs
	BOOST_CHECK_EQUAL( (node+3)->index,   -1  );
	BOOST_CHECK_EQUAL( (node+3)->value,   -1 );// 


	free( node );
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_node_ptr_to_ptr )
{
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	
	struct svm_node** node_matrix;

	LibSvmAdapter l;
	node_matrix = l.create_svm_node_ptr_to_ptr( f.svm_train_matrix );
	
	//test first, second, last rows
	//test 0,0 0,1 0,2  indices and values
	struct svm_node* row_zero = *node_matrix;
	BOOST_CHECK_EQUAL( row_zero->value, f.svm_train_matrix.at(0)->at(0) );
	BOOST_CHECK_EQUAL( (row_zero+1)->value, f.svm_train_matrix.at(0)->at(1) );
	BOOST_CHECK_EQUAL( (row_zero+2)->value, f.svm_train_matrix.at(0)->at(2) );
	BOOST_CHECK_EQUAL( (row_zero+3)->value, -1 );//check termination symbol
	free( row_zero );

	struct svm_node* row_one = *(node_matrix+1);
	BOOST_CHECK_EQUAL( row_one->value, f.svm_train_matrix.at(1)->at(0) );
	BOOST_CHECK_EQUAL( (row_one+1)->value, f.svm_train_matrix.at(1)->at(1) );
	BOOST_CHECK_EQUAL( (row_one+2)->value, f.svm_train_matrix.at(1)->at(2) );
	BOOST_CHECK_EQUAL( (row_one+3)->value, -1 );//check termination symbol
	free( row_one );

	struct svm_node* row_two = *(node_matrix+2);
	BOOST_CHECK_EQUAL( row_two->value, f.svm_train_matrix.at(2)->at(0) );
	BOOST_CHECK_EQUAL( (row_two+1)->value, f.svm_train_matrix.at(2)->at(1) );
	BOOST_CHECK_EQUAL( (row_two+2)->value, f.svm_train_matrix.at(2)->at(2) );
	BOOST_CHECK_EQUAL( (row_two+3)->value, -1 );//check termination symbol
	free( row_two );

	//free( *node_matrix );
	//free( *(node_matrix+1) );
	//free( *(node_matrix+2) );
	free( node_matrix );
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_problem )
{
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	
	struct svm_problem* problem;

	LibSvmAdapter l;
	problem = l.create_svm_problem( f.svm_train_matrix );
		
	BOOST_CHECK_EQUAL( problem->l, f.svm_train_matrix.size() );

	//check on x
	struct svm_node** node_matrix = problem->x;
	struct svm_node* row_zero = *node_matrix;
	BOOST_CHECK_EQUAL( row_zero->value, f.svm_train_matrix.at(0)->at(0) );
	BOOST_CHECK_EQUAL( (row_zero+1)->value, f.svm_train_matrix.at(0)->at(1) );
	BOOST_CHECK_EQUAL( (row_zero+2)->value, f.svm_train_matrix.at(0)->at(2) );
	
	//check end points
	BOOST_CHECK_EQUAL( (row_zero+3)->value, -1 );
	//free( row_zero );

	//check on y
	double* node = problem->y;
	BOOST_CHECK_EQUAL( -1, *node  );

	//check for memory leaks
	l.delete_svm_problem( problem );
	
	//struct svm_problem* null_pointer = NULL;
	//BOOST_CHECK_EQUAL( null_pointer, problem );
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_parameter )
{
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

	LibSvmAdapter l;
	struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );
	
	Options o;

	struct svm_parameter* param = l.create_svm_parameter( problem, o );

	BOOST_CHECK_EQUAL( param->C, 1  );
	BOOST_CHECK_EQUAL( param->gamma, 1 );//
	//BOOST_CHECK_CLOSE( param->gamma, 0.33333333, 0.0001 );//
	
	l.delete_svm_parameter( param );

	o.log_cost = 3;
	o.log_gamma = -3;
	struct svm_parameter* param2 = l.create_svm_parameter( problem, o  );

	BOOST_CHECK_EQUAL( param2->C, 8  );
	BOOST_CHECK_EQUAL( param2->gamma,  0.125 ); 

	l.delete_svm_parameter( param2 );

	o.log_cost = 32;
	o.log_gamma = -31;
	struct svm_parameter* param3 = l.create_svm_parameter( problem, o  );

	BOOST_CHECK_EQUAL( param3->C, 4294967296ULL  );
	BOOST_CHECK_EQUAL( param3->gamma,  0.0000000004656612873077392578125 );

	l.delete_svm_parameter( param3 );

	o.log_cost = 33;
	o.log_gamma = -32;
	struct svm_parameter* param4 = l.create_svm_parameter( problem, o );

	BOOST_CHECK_EQUAL( param4->C, 8589934592ULL  );
	BOOST_CHECK_EQUAL( param4->gamma,  0.00000000023283064365386962890625 );

	l.delete_svm_parameter( param4 );

	o.log_cost = 0;
	o.log_gamma = -1;
	struct svm_parameter* param5 = l.create_svm_parameter( problem, o  );

	BOOST_CHECK_EQUAL( param5->C, 1  );
	BOOST_CHECK_EQUAL( param5->gamma,  .5 );

	l.delete_svm_parameter( param5 );

	l.delete_svm_problem( problem );

}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_train_model_pr_small3 )
{
	//test. um, what's the model look like?
	//similar to this. so let's create in python libsvm by hand
	//and compare?

	//also search memory leaks

	//create a controlled svmwumatrix from small3
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

	Options o;
	LibSvmAdapter l;
	struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
	struct svm_parameter* param = l.create_svm_parameter( problem, o );
	struct svm_model* train_model;//svm.h

	train_model = l.train( f.svm_train_matrix, param, problem );

	//BEGIN fix category label problem
	//test for 1 and -1 in correct places

	double* testee = problem->y;

	//NFV threshold 2
	BOOST_CHECK_EQUAL( -1, *testee );//2.2
	BOOST_CHECK_EQUAL( 1, *(testee+1) );//.8
	BOOST_CHECK_EQUAL( 1, *(testee+2) );//1.2

	//END

	//save and eyeball it. easiest first step.
	svm_save_model( "../data/LibSvmAdapterTest/PR_small3_model.txt", train_model );//svm.h

	//check model size somehow
	//by counting number of nodes in train_model->SV
/**
//TODO IF YOU WANT TO TEST DIRECTLY ON train_model, you have to move the definition
of struct train_model into svm.h It's currently in svm.cpp for some stupid
reason, so it won't link when you try to access the members vars of it
*/
	
	//	BOOST_CHECK_EQUAL( f.svm_train_matrix.size(), train_model->l );
//	BOOST_CHECK_EQUAL( f.svm_train_matrix.at(0)->at(0), train_model->SV[0]->value );

	svm_destroy_model( train_model );//DELETE SVM_MODEL
	l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
	l.delete_svm_problem( problem );//DELETESVM_PROBLEM
	
}

#if defined  LONG_TESTS || defined TEST_ALL
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_train_model_pr_full )
{

	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv" );

	Options o;
	LibSvmAdapter l;
	struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
	struct svm_parameter* param = l.create_svm_parameter( problem, o);
	struct svm_model* train_model;//svm.h

	train_model = l.train( f.svm_train_matrix, param, problem );

	Log::append( "LibSvmAdapterTest_train_model_pr_full" );
	Log::append( "train_model = l.train( f.svm_train_matrix, param, problem );" );
	Log::append( "f.svm_train_matrix.size = " + boost::lexical_cast<std::string>(f.svm_train_matrix.size() ) );
	Log::append( "problem->l" +  boost::lexical_cast<std::string>(problem->l)  );
	

//	save and eyeball it. easiest first step.
	svm_save_model( "../data/LibSvmAdapterTest/PR_model-NFV.txt", train_model );//svm.h

	svm_destroy_model( train_model );//DELETE SVM_MODEL
	l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
	l.delete_svm_problem( problem );//DELETESVM_PROBLEM

}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_pr_full)
{
	//well, this is the theory part, right? does this svm thing work?
	//we can run predict here with a known sequence that is included
	//in the training set.
	//However, a true or false won't mean jack.
	//but it's good to check for memory leaks, etc.

	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv" );
	LibSvmAdapter l;
	Options o;

	//TODO make something more useful out of this?
	//std::vector<bool> results;
	ExperimentResultSet result_set;
	l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result_set );

	//BOOST_CHECK_EQUAL( results.at(0), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o ) );
	//BOOST_CHECK_EQUAL( results.at(1), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o ) );
}

#endif

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_pr_small3 )
{
	//well, this is the theory part, right? does this svm thing work?
	//we can run predict here with a known sequence that is included
	//in the training set.
	//However, a true or false won't mean jack.
	//but it's good to check for memory leaks, etc.

	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l;
	Options o;

	//TODO make something more useful out of this?
	//std::vector<bool> results;
	ExperimentResultSet result_set;
	l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result_set );
}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_many_pr_small3 )
//{
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	Options o;
//	
//	std::vector<bool> results;
//
//	results = l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o );
//
//	BOOST_CHECK_EQUAL( results.at(0), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o ) );
//	BOOST_CHECK_EQUAL( results.at(1), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o ) );
//}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_with_probability_single_pr_small3 )
//{
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	Options o;
//
//	bool result;//prediction of class
//	std::vector<double> probabilities;//container of probability of each class occurring
//
//	result = l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], o, probabilities );
//	
//	//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
//	BOOST_CHECK_EQUAL( probabilities.size(), 2 );
//
//	//test for result being equal to the class with the greatest probability
//	//if( result < 0 && result > 1)
//	//{
//	//	BOOST_FAIL( "Result of predict_susceptibility_with_probabilities out of range." );
//	//}
//
//	
//	//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
//	// so a -1 means that the values were never set properly by libsvm.
//	for(int i=0; i< probabilities.size(); i++ )
//	{
//		BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( i ) ) );
//	}
//
//	//test for result class having greater or equal probability
//	if( result == false )
//	{
//		BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( result ) ) (probabilities.at( 1 ) ) );
//	}
//
//	if( result == true )
//	{
//		BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( result ) ) ( probabilities.at( 0 ) ) );
//	}
//
//	//CHECK same factors of a second prediction
//
//	bool result2;//prediction of class
//	std::vector<double> probabilities2;//container of probability of each class occurring
//	result2 = l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[1], o,
//		probabilities2);
//
//	//test for result2 being equal to the class with the greatest probability
//	//if( result2 < 0 && result2 > 1)
//	//{
//	//	BOOST_FAIL( "result2 of predict_susceptibility_with_probabilities out of range." );
//	//}
//
//	//test for result2 class having greater or equal probability
//	if( result2 == false )
//	{
//		BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( result2 ) ) ( probabilities.at( 1 ) ) );
//	}
//
//	if( result2 == true )
//	{
//		BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( result2 ) ) ( probabilities.at( 0 ) ) );
//	}
//}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_single_pr_small3_cost_gamma )
//{
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//
//	double log_cost = 3;
//	double log_gamma = -2;
//
//	//TODO make something more useful out of this?
//	Options o;
//	bool result;
//	result = l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o, log_cost, log_gamma );
//	result = l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o, log_cost, log_gamma );
//}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_many_pr_small3_cost_gamma )
//{
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	Options o;
//	
//	std::vector<bool> prediction_results;
//
//	//USE CUSTOM C,G
//	double log_cost = -30;
//	double log_gamma = -30;
//
//	prediction_results = l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, log_cost, log_gamma );
//
//	BOOST_CHECK_EQUAL( prediction_results.at(0), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o, log_cost, log_gamma ) );
//	BOOST_CHECK_EQUAL( prediction_results.at(1), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o, log_cost, log_gamma ) );
//
//	//did the predictions work? 
//	//this is a crap test. ditch it.
//	//BOOST_CHECK_EQUAL( prediction_results.at(0), f.svm_predictee_matrix[0]->known_susceptibility() );
//	//BOOST_CHECK_EQUAL( prediction_results.at(1), f.svm_predictee_matrix[1]->known_susceptibility() );
//
//
//	//USE DEFAULTS C,G
//	std::vector<bool> results_defaults;
//	results_defaults = l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o );
//
//	//did the predictions work?
//	BOOST_CHECK_EQUAL( results_defaults.at(0), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o  ) );
//	BOOST_CHECK_EQUAL( results_defaults.at(1), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o  ) );
//
//
//	//this may be too few predictions (2) for the c,g to mess it up.
//	for( size_t i=0; i < prediction_results.size(); i++ )
//	{
//		//BOOST_CHECK_PREDICATE( std::not_equal_to<bool>(), ( results.at(i) ) ( results_defaults.at(i) ) );
//	}
//}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_vs_libsvm_predict )
{
	//setup a call directly to svm_predict
    //begin setup
	LibSvmAdapterTest_Fixture f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l2;
	Options o2;	
	
	struct svm_problem* problem = l2.create_svm_problem( f2.svm_train_matrix );//NEW SVM_PROBLEM
	struct svm_parameter* param = l2.create_svm_parameter( problem, o2 );//NEW SVM_PARAMETER
	struct svm_model* train_model = l2.train( f2.svm_train_matrix, param, problem );//NEW SVM_MODEL

	double svm_result;

	struct svm_node* predictee_ = l2.create_svm_node_ptr( f2.svm_predictee_matrix[0] );//CALLOC SVM_NODE*
	svm_result = svm_predict( train_model, predictee_ );
	free( predictee_ );//FREE SVM_NODE*

	svm_destroy_model( train_model );//DELETE SVM_MODEL
	l2.delete_svm_parameter( param );//DELETE SVM_PARAMETER
	l2.delete_svm_problem( problem );//DELETESVM_PROBLEM
	//end setup

	//setup use of libsvmadapter version
	//begin setup 2
	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l;
	Options o;
	//end setup 2

	//I chose labels for libsvm.
	// 1 susceptible
	// -1 not susceptible.
	// I did not use 0,1,2,3, etc.

//	std::vector<bool> results;
	ExperimentResultSet result_set;
	l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result_set );

	//TODO make cleaner interface to data points in ExperimentResults
	if( svm_result == -1 )
	{
		bool is_libsvm_susceptible = false;

		//check that these two are opposite of each other.
		BOOST_CHECK_EQUAL( is_libsvm_susceptible, result_set.front()->data.data_points.front()->predicted_susceptibility );
	}
	else if( svm_result == 1 )
	{
		bool is_libsvm_susceptible = true;

		//check that these two are opposite of each other.
		BOOST_CHECK_EQUAL( is_libsvm_susceptible, result_set.front()->data.data_points.front()->predicted_susceptibility  );
	}
	else
	{
		BOOST_FAIL("svm_predict should have returned -1 or 1");
	}
}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_probabilities_vs_libsvm_predict_probabilites )
//{
//		//setup a call directly to svm_predict
//    //begin setup
//	LibSvmAdapterTest_Fixture f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l2;
//	Options o2;	
//	
//	struct svm_problem* problem = l2.create_svm_problem( f2.svm_train_matrix );//NEW SVM_PROBLEM
//	struct svm_parameter* param = l2.create_svm_parameter( problem, o2, o2.log_cost, o2.log_gamma );//NEW SVM_PARAMETER
//	struct svm_model* train_model = l2.train( f2.svm_train_matrix, param, problem );//NEW SVM_MODEL
//
//	double svm_result;
//	double prob_estimates[ 2 ];//prob_estimates MEM
//
//	struct svm_node* predictee_ = l2.create_svm_node_ptr( f2.svm_predictee_matrix[0] );//CALLOC SVM_NODE*
//	svm_result = svm_predict_probability( train_model, predictee_, prob_estimates );
//	free( predictee_ );//FREE SVM_NODE*
//
//	svm_destroy_model( train_model );//DELETE SVM_MODEL
//	l2.delete_svm_parameter( param );//DELETE SVM_PARAMETER
//	l2.delete_svm_problem( problem );//DELETESVM_PROBLEM
//	//end setup
//
//	//setup use of libsvmadapter version
//	//begin setup 2
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	Options o;
//	std::vector<double> probabilities_output;
//	//end setup 2
//
//
//	if( svm_result == -1 )
//	{
//		//lower class means less resstance than a higher class
//		//-1 means class 0 in libsvm_readme.txt
//		// 1 means class 1
//		bool is_libsvm_susceptible = true;
//
//		//check that these two are opposite of each other.
//		BOOST_CHECK_EQUAL( is_libsvm_susceptible, l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], o, probabilities_output ) );
//	}
//	else if( svm_result == 1 )
//	{
//		//lower class means less resistance than a higher class
//		//-1 means class 0 in libsvm_readme.txt
//		// 1 means class 1
//		bool is_libsvm_susceptible = false;
//
//		//check that these two are opposite of each other.
//		BOOST_CHECK_EQUAL( is_libsvm_susceptible,
//			l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], o, probabilities_output ) );
//	}
//	else
//	{
//		BOOST_FAIL("svm_predict should have returned -1 or 1 for a 2 class problem");
//	}
//}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_probabilities_vs_predict_susceptibility )
//{
//
//	//begin setup predict_susceptibility_with_probabilities 
//	//Options o;
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	f.preproc_options.probability = 0;
//	std::vector<double> probabilities_output;
//	//end setup 1
//
//	//begin setup predict_susceptibility (no probability)
//	LibSvmAdapterTest_Fixture f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l2;
//	Options o2;
//	o2.probability = 0;//don't use probability
//	//end setup 1
//
////l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], f.preproc_options, probabilities_output );
//	
////BOOST_CHECK_EQUAL( l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], f.preproc_options, probabilities_output ),
//	//	l2.predict_susceptibility( f2.svm_train_matrix, f2.svm_predictee_matrix[0], o2 ) );
//}

//BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_probabilities_probability_param )
//{
//	//HOW MUCH impact does the probability param switch have on testing with probabilities?
//
//	//begin setup predict_susceptibility_with_probabilities 
//	LibSvmAdapterTest_Fixture f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l;
//	Options o;
//	o.probability = 1;//turn on probability param switch
//	std::vector<double> probabilities_output;
//	//end setup 1
//
//	//begin setup predict_susceptibility_with_probabilities (no probability)
//	LibSvmAdapterTest_Fixture f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
//	LibSvmAdapter l2;
//	Options o2;
//	o2.probability = 0;//turn off probability param switch
//	std::vector<double> probabilities_output2;
//	//end setup 1
//
//	BOOST_CHECK_EQUAL( l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], o, probabilities_output ),
//		l2.predict_susceptibility_with_probabilities( f2.svm_train_matrix, f2.svm_predictee_matrix[0], o2, probabilities_output2 ) );
//
//}
BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  LIBSVMADAPTER_TEST || defined TEST_ALL
#endif //LIBSVMADAPTERTEST_HPP
