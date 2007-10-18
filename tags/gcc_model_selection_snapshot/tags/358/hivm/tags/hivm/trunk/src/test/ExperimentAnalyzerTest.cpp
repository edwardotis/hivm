#ifndef EXPERIMENTANALYZERTEST_HPP
#define EXPERIMENTANALYZERTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  EXPERIMENTANALYZER_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/lexical_cast.hpp> //safe numerical to string casts
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values

//#include <string>
//#include <vector>


#include "../ExperimentAnalyzer.hpp"
#include "../ExperimentResult.hpp"
#include "../PreProcessor.hpp"
#include "../SelfTestExperiment.hpp"
#include "../SvmMachine.hpp"
#include "../Log.hpp"
#include "../FunctionObjects.cpp"//functor for random integer in random_shuffle
#include "../CrossValidationExperiment.hpp"
#include "../ValidationExperiment.hpp"

struct ExperimentAnalyzerTest_Fixture
{
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	SvmMachine machine;
	SelfTestExperiment self_test_exp;
	CrossValidationExperiment cross_val_exp;
	ExperimentResultSet result_set;
	Options options;

	////variables grid search
	//double log_cost_low;
	//double log_cost_high;
	//double log_cost_increment;

	//double log_gamma_low;
	//double log_gamma_high;
	//double log_gamma_increment;

	////ctor cross val
	ExperimentAnalyzerTest_Fixture(
		std::string test_data_path,
		std::string wild_seq, 
		std::string drug, 
		double threshold,
		std::string experiment_type,
		size_t seed
		)
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

	//log_cost_low			= -5;
	//log_cost_high			=  5;
	//log_cost_increment		=  2;

	//log_gamma_low			= -5;
	//log_gamma_high			=  5;
	//log_gamma_increment		=  2;

	Log::append( "ExperimentAnalyzerTest__Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	}


	////ctor SelfTest
	//ExperimentAnalyzerTest_Fixture(
	//	std::string test_data_path, 
	//		std::string wild_seq, 
	//		std::string drug, 
	//		double threshold,
	//		std::string experiment_type,
	//		bool is_self_test )
	//	{
	//	p.parseInputFiles(
	//	test_data_path,
	//	wild_seq,
	//	drug,
	//	threshold,//threshold
	//	input_train_set
	//	);
	//
	//	//setup result set
	//	result_set.set_params(
	//		experiment_type,
	//		drug,
	//		threshold,
	//		wild_seq,
	//		test_data_path,
	//		input_train_set.size()
	//		);

	////log_cost_low			= -5;
	////log_cost_high			=  5;
	////log_cost_increment		=  2;

	////log_gamma_low			= -5;
	////log_gamma_high			=  5;
	////log_gamma_increment		=  2;

	//Log::append( "ExperimentAnalyzerTest__Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	//}

		//dtor
	~ExperimentAnalyzerTest_Fixture(){}
};

BOOST_AUTO_TEST_SUITE( ExperimentAnalyzerTests );

//check divide by zero and empties.
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_all_false )
{
	//test the stats functions
	//create an ExperimentResult here with simple
	//datapoint set. Do, the math, and make sure the
	//app matches on the stats.


//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	false, false, "" );//predicted, actual
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	false, false, "" );
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	false, false, "" );
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	false, false, "" );
//predicted, actual

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

ExperimentAnalyzer ea;
//ALL STATS RETURN -1 FOR DIVIDE BY ZERO
//-1 == Undefined in output (add this to header row)

//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc.
//(0.5/0.5) / (0.5/4.5) = 1/.111111111 = 9.00000000009
//1/.1 =  10, 1/.11 = 9.0909090909090909090909090909091 

BOOST_CHECK_CLOSE( ea.diagnostic_odds_ratio( er ), 9.0, .0001 );

// n1 = Positive class, susceptible = TP + FN = 4
// n2 = Negative class, resistant  = TN + FP = 0
// pmax = min(n1, n2)/(n1+n2)
// n2/ ( TP + FN + TN + FP )
// 0/4 = 0
BOOST_CHECK_EQUAL( ea.pmax( er ), 0 );

//	(TP+TN)/N
//	(0 + 4)/4 = 1	
BOOST_CHECK_EQUAL( ea.accuracy( er ), 1 );

//	(FP+FN)/N
//	(0 + 0)/4 = 0
BOOST_CHECK_EQUAL( ea.error_rate( er ), 0 );

//p-error = error rate / pmax
// 0/0 = undefined
BOOST_CHECK_EQUAL( ea.p_error( er ), -1 );

// TP/( TP + FN )
// 0/ ( 0 +  0 ) = Undefined, -1
BOOST_CHECK_EQUAL( ea.tpr( er ), -1 );

// 1.0 - [TN/( FP + TN )]
// 1.0 - [4/( 0 + 4 )] = 0 
BOOST_CHECK_EQUAL( ea.fpr( er ), 0 );

// FP/(TN + FN)
// 0 / (4 + 0) 
BOOST_CHECK_EQUAL( ea.fpr( er ), 0 );

// TN/( FP + TN ) == specificity
// 4/( 0 + 4 ) = 1
BOOST_CHECK_EQUAL( ea.tnr( er ), 1 );

// 1.0 - [TP/( TP + FN )]
// 1.0 - [0 / (0 + 0)] = undefined, -1
BOOST_CHECK_EQUAL( ea.fnr( er ), -1 );

// FN/(TP + FP)
// 0/(0 + 0 )
BOOST_CHECK_EQUAL( ea.fnr( er ), -1 );

// TP/( TP + FN )
// 0 / (0 + 0 ) = Undefined, -1
BOOST_CHECK_EQUAL( ea.sensitivity( er ), -1 );

// TN/( FP + TN ) = TNR
// 4/( 0 + 4 ) = 1
BOOST_CHECK_EQUAL( ea.specificity( er ), 1 );

//  TP/(TP+FP)
// 0 / (0+0) = Undefined, -1
BOOST_CHECK_EQUAL( ea.positive_predictive_value( er ), -1 );

//  TN/(TN+FN)
//	4/( 4+ 0) = 1
BOOST_CHECK_EQUAL( ea.negative_predictive_value( er ), 1 );


//clean up
delete er;
}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_no_data_points )
{
	ExperimentResult *er = new ExperimentResult(0,0);
	ExperimentAnalyzer ea;

//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc.
//(0.5/0.5) / (0.5/0.5) = 1/1 = 1
BOOST_CHECK_EQUAL( ea.diagnostic_odds_ratio( er ), 1 );

//(TN + FP) / ( TP + FN + TN + FP )
// 0+0/0 = undefined
BOOST_CHECK_EQUAL( ea.pmax( er ), -1 );


//	(TP+TN)/N
BOOST_CHECK_EQUAL( ea.accuracy( er ), -1 );

//	(FP+FN)/N
BOOST_CHECK_EQUAL( ea.error_rate( er ), -1 );

// TP/( TP + FN )
BOOST_CHECK_EQUAL( ea.tpr( er ), -1 );

// 1.0 - [TN/( FP + TN )]
BOOST_CHECK_EQUAL( ea.fpr( er ), -1 );

// TN/( FP + TN ) == specificity
BOOST_CHECK_EQUAL( ea.tnr( er ), -1 );

// 1.0 - [TP/( TP + FN )]
BOOST_CHECK_EQUAL( ea.fnr( er ), -1 );

// TP/( TP + FN )
BOOST_CHECK_EQUAL( ea.sensitivity( er ), -1 );

// TN/( FP + TN ) = TNR
BOOST_CHECK_EQUAL( ea.specificity( er ), -1 );

//  TP/(TP+FP)
BOOST_CHECK_EQUAL( ea.positive_predictive_value( er ), -1 );

//  TN/(TN+FN)
BOOST_CHECK_EQUAL( ea.negative_predictive_value( er ), -1 );


//clean up
delete er;

}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_all_true )
{
	//test the stats functions
	//create an ExperimentResult here with simple
	//datapoint set. Do, the math, and make sure the
	//app matches on the stats.


//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	true, true, "" );//predicted, actual
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	true, true, "" );
//predicted, actual

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

ExperimentAnalyzer ea;
//ALL STATS RETURN -1 FOR DIVIDE BY ZERO
//-1 == Undefined in output (add this to header row)

//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc.
//(4.5/0.5) / (0.5/0.5) = 9/1 = 9
BOOST_CHECK_CLOSE( ea.diagnostic_odds_ratio( er ), 9.0, .0001 );

// n1 = Positive class, susceptible = TP + FN = 0
// n2 = Negative class, resistant  = TN + FP = 4
// pmax = min(n1, n2)/(n1+n2)
// n1 / ( TP + FN + TN + FP )
// 0/4 = 0
BOOST_CHECK_EQUAL( ea.pmax( er ), 0 );


//	(TP+TN)/N
//	(4 + 0)/4 = 1	
BOOST_CHECK_EQUAL( ea.accuracy( er ), 1 );

//	(FP+FN)/N
//	(0 + 0)/4 = 0
BOOST_CHECK_EQUAL( ea.error_rate( er ), 0 );

//p-error = error rate / pmax
// 0/0 = undefined
BOOST_CHECK_EQUAL( ea.p_error( er ), -1 );

// TP/( TP + FN )
// 4/ ( 4 +  0 )
BOOST_CHECK_EQUAL( ea.tpr( er ), 1 );

// 1.0 - [TN/( FP + TN )]
// 1.0 - [0/( 0 + 0 )]

// FP/(TN + FN)
// 1/ 
BOOST_CHECK_EQUAL( ea.fpr( er ), -1 );

// TN/( FP + TN ) == specificity
// 0/( 0 + 0 )
BOOST_CHECK_EQUAL( ea.tnr( er ), -1 );

// 1.0 - [TP/( TP + FN )]
// 1.0 - [4 / (4 + 0)]
BOOST_CHECK_EQUAL( ea.fnr( er ), 0 );

// TP/( TP + FN )
// 4 / (4 + 0 )
BOOST_CHECK_EQUAL( ea.sensitivity( er ), 1 );

// TN/( FP + TN ) = TNR
// 0/( 0 + 0 ) = 0
BOOST_CHECK_EQUAL( ea.specificity( er ), -1 );

//  TP/(TP+FP)
// 4 / (4+0) = 1
BOOST_CHECK_EQUAL( ea.positive_predictive_value( er ), 1 );

//  TN/(TN+FN)
//	0/( 0 + 0)
BOOST_CHECK_EQUAL( ea.negative_predictive_value( er ), -1 );

//clean up
delete er;
}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_one_of_each )
{
	//test the stats functions
	//create an ExperimentResult here with simple
	//datapoint set. Do, the math, and make sure the
	//app matches on the stats.


//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	true, false, "" );//predicted, actual
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	false, true, "" );
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	false, false, "" );
//predicted, actual
//1 of each

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

ExperimentAnalyzer ea;
//ALL STATS RETURN -1 FOR DIVIDE BY ZERO
//-1 == Undefined in output (add this to header row)

//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc.
//(1.5/1.5) / (1.5/1.5) = 1
BOOST_CHECK_EQUAL( ea.diagnostic_odds_ratio( er ), 1 );

//(TN + FP) / ( TP + FN + TN + FP )
// 1+1/4 = .5
BOOST_CHECK_CLOSE( ea.pmax( er ), .5, .01 );

//	(TP+TN)/N
//	(1 + 1)/4	
BOOST_CHECK_EQUAL( ea.accuracy( er ), .5 );

//	(FP+FN)/N
//	(1 + 1)/4
BOOST_CHECK_EQUAL( ea.error_rate( er ), .5 );

//p-error = error rate / pmax
// .5/.5 = 1
BOOST_CHECK_EQUAL( ea.p_error( er ), 1 );

// TP/( TP + FN )
// 1/ ( 1 +  1)
BOOST_CHECK_EQUAL( ea.tpr( er ), .5 );

// 1.0 - [TN/( FP + TN )]
// 1.0 - [1/( 1 + 1 )]
//FP/(TN + FP)
//1/(1+1)
BOOST_CHECK_EQUAL( ea.fpr( er ), .5 );

// TN/( FP + TN ) == specificity
// 1/( 1 + 1 )
BOOST_CHECK_EQUAL( ea.tnr( er ), .5 );

// 1.0 - [TP/( TP + FN )]
// 1.0 - [1 / (1 + 1)]
//FN/(TP + FN)
//1/(1+1)
BOOST_CHECK_EQUAL( ea.fnr( er ), .5 );

// TP/( TP + FN )
// 1 / (1 + 1 )
BOOST_CHECK_EQUAL( ea.sensitivity( er ), .5 );

// TN/( FP + TN ) = TNR
// 1/( 1 + 1 )
BOOST_CHECK_EQUAL( ea.specificity( er ), .5 );

//  TP/(TP+FP)
// 1 / (1+1)
BOOST_CHECK_EQUAL( ea.positive_predictive_value( er ), .5 );

//  TN/(TN+FN)
//	1/( 1 + 1)
BOOST_CHECK_EQUAL( ea.negative_predictive_value( er ), .5 );

//clean up
delete er;
}



BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_prune )
{
	//make a result set with some duplicate fpr's and diff tpr's.
	//see if prune works on it.

//TPR = 2/(2) = 1 TP/( TP + FN )
//FPR = FP/(TN + FP) =
//      0/2 = 0;
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	false, false, "" );//TN
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	false, false, "" );//TN

//TPR = 1/(1) = 1
//FPR = 1/2   = .5 FP/(TN + FP)
struct ExperimentResult::data_point* dp02 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp12 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp22 = new ExperimentResult::data_point(
	false, false, "" );//TN
struct ExperimentResult::data_point* dp32 = new ExperimentResult::data_point(
	true, false, "" );//FP

//TPR = 2/(3) = .66666
//FPR = 1/1   = 1 FP/(TN + FP)
struct ExperimentResult::data_point* dp03 = new ExperimentResult::data_point(
	false, true, "" );//FN
struct ExperimentResult::data_point* dp13 = new ExperimentResult::data_point(
	true, false, "" );//FP
struct ExperimentResult::data_point* dp23 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp33 = new ExperimentResult::data_point(
	true, true, "" );//TP

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

ExperimentResult *er2 = new ExperimentResult(0,0);
er2->datum.data_points.push_back( dp02 );
er2->datum.data_points.push_back( dp12 );
er2->datum.data_points.push_back( dp22 );
er2->datum.data_points.push_back( dp32 );

ExperimentResult *er3 = new ExperimentResult(0,0);
er3->datum.data_points.push_back( dp03 );
er3->datum.data_points.push_back( dp13 );
er3->datum.data_points.push_back( dp23 );
er3->datum.data_points.push_back( dp33 );

ExperimentResultSet input_set;//handles memory destruction for all of the above
input_set.push_back( er3 );//fpr 1
input_set.push_back( er );//fpr 0
input_set.push_back( er2 );//fpr .5

ExperimentAnalyzer ea;
//
////verify unsorted fpr's
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(0) ), 1 );
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(1) ), 0 );
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(2) ), .5 );

ExperimentResultSet pruned_set;
ea.prune( input_set, pruned_set );

//check that input was not sorted by fpr
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(0) ), 1 );
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(1) ), 0 );
BOOST_CHECK_EQUAL( ea.fpr( input_set.at(2) ), .5 );

//verify pruning of output set.
BOOST_CHECK_EQUAL( pruned_set.size(), 3 );

BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(0) ),  0 );
BOOST_CHECK_EQUAL( ea.tpr( pruned_set.at(0) ), 1 );

BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(1) ), .5 );
BOOST_CHECK_EQUAL( ea.tpr( pruned_set.at(1) ), 1 );

BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(2) ), 1 );
BOOST_CHECK_CLOSE( ea.tpr( pruned_set.at(2) ), .666666, .01 );
//BOOST_CHECK_EQUAL( ea.tpr( pruned_set.at(1) ), .666 );
}

#if defined LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_prune2 )
{
	//set variables
	std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = true;
	size_t seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Cross_Val_Experiment",
		seed
		);	

	Options o;//defaults

	//run experiment through grid search
	f.machine.parameter_search(
		f.cross_val_exp,
		f.input_train_set,
		o,
		1,//f.log_cost_low,
		1,//f.log_cost_high,
		1,//f.log_cost_increment,
		-5,//f.log_gamma_low,
		3,//f.log_gamma_high,
		2,//f.log_gamma_increment,
		f.result_set
		);

/** 
OLD
KNOWN RESULTS on NFV 50 BEFORE PRUNING
//Log_Cost Log_Gamma	 TPR	 FPR
//1	-5	0.894737	0.928571
//1	-3	0.947368	0.714286
//1	-1	0.894737	0.571429
//1	1	0.947368	0.785714
//1	3	1.000000	0.928571

KNOWN RESULTS on NFV 50 BEFORE PRUNING
UPDATED b/c HIVM shuffle is switched to portable BOOST random number generator now.
Log_Cost	 Log_Gamma	 TPR	 FPR
1.0000	-5.0000	0.6250	1.0000
1.0000	-3.0000	0.6875	0.6429
1.0000	-1.0000	0.5625	0.5000
1.0000	1.0000	0.4375	0.5000
1.0000	3.0000	0.5625	0.7857
*/

//analyze
ExperimentAnalyzer ea;

BOOST_CHECK_EQUAL( 5, f.result_set.size() );

//VERIFY THESE 'KNOWN' RESULTS FIRST.


BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(0) ), 1.0, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(0) ), 0.6250, 0.1 );//

BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(1) ), 0.6429, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(1) ), 0.6875, 0.1 );//

BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(2) ), 0.5000, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(2) ), 0.5625, 0.1 );//

BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(3) ), 0.5000, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(3) ), 0.4375, 0.1 );//

BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(4) ), 0.7857, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(4) ), 0.5625, 0.1 );//

//BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(0) ), 0.928571, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(0) ), 0.894737, 0.1 );//
//
//BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(1) ), 0.714286, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(1) ), 0.947368, 0.1 );//
//
//BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(2) ), 0.571429, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(2) ), 0.894737, 0.1 );//
//
//BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(3) ), 0.785714, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(3) ), 0.947368, 0.1 );//
//
//BOOST_CHECK_CLOSE( ea.fpr( f.result_set.at(4) ), 0.928571, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( f.result_set.at(4) ), 1.0, 0.1 );//

ExperimentResultSet pruned_result_set;
ea.prune( f.result_set, pruned_result_set );//prune

BOOST_CHECK_EQUAL( 4, pruned_result_set.size() );


BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(0) ), 0.5000, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(0) ), 0.5625, 0.1 );//
BOOST_CHECK_EQUAL( pruned_result_set.at(0)->log_cost, 1 );
BOOST_CHECK_EQUAL( pruned_result_set.at(0)->log_gamma, -1 );

BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(1) ), 0.6429, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(1) ), 0.6875, 0.1 );//
BOOST_CHECK_EQUAL( pruned_result_set.at(1)->log_cost, 1 );
BOOST_CHECK_EQUAL( pruned_result_set.at(1)->log_gamma, -3 );

BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(2) ), 0.7857, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(2) ), 0.5625, 0.1 );//
BOOST_CHECK_EQUAL( pruned_result_set.at(2)->log_cost, 1 );
BOOST_CHECK_EQUAL( pruned_result_set.at(2)->log_gamma, 3 );

BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(3) ), 1.0000, 0.1 );//
BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(3) ), 0.6250, 0.1 );//
BOOST_CHECK_EQUAL( pruned_result_set.at(3)->log_cost, 1 );
BOOST_CHECK_EQUAL( pruned_result_set.at(3)->log_gamma, -5 );
//
//BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(0) ), 0.571429, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(0) ), 0.894737, 0.1 );//
//BOOST_CHECK_EQUAL( pruned_result_set.at(0)->log_cost, 1 );
//BOOST_CHECK_EQUAL( pruned_result_set.at(0)->log_gamma, -1 );
//
//BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(1) ), 0.714286, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(1) ), 0.947368, 0.1 );//
//BOOST_CHECK_EQUAL( pruned_result_set.at(1)->log_cost, 1 );
//BOOST_CHECK_EQUAL( pruned_result_set.at(1)->log_gamma, -3 );
//
//BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(2) ), 0.785714, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(2) ), 0.947368, 0.1 );//
//BOOST_CHECK_EQUAL( pruned_result_set.at(2)->log_cost, 1 );
//BOOST_CHECK_EQUAL( pruned_result_set.at(2)->log_gamma, 1 );
//
//BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(3) ), 0.928571, 0.1 );//
//BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(3) ), 1.0, 0.1 );//
//BOOST_CHECK_EQUAL( pruned_result_set.at(3)->log_cost, 1 );
//BOOST_CHECK_EQUAL( pruned_result_set.at(3)->log_gamma, 3 );

}

#endif

//test by looking at file for now.
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_roc_data )
{
//TPR = 2/(2) = 1 TP/( TP + FN )
//FPR = 0/2   = 0
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	false, false, "" );//TN
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	false, false, "" );//TN

//TPR = 1/(1) = 1
//FPR = 1/2   = 1 FP/(TN + FN)
struct ExperimentResult::data_point* dp02 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp12 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp22 = new ExperimentResult::data_point(
	false, false, "" );//TN
struct ExperimentResult::data_point* dp32 = new ExperimentResult::data_point(
	true, false, "" );//FP

//TPR = 2/(3) = .66666
//FPR = 1/1   = 1
struct ExperimentResult::data_point* dp03 = new ExperimentResult::data_point(
	false, true, "" );//FN
struct ExperimentResult::data_point* dp13 = new ExperimentResult::data_point(
	true, false, "" );//FP
struct ExperimentResult::data_point* dp23 = new ExperimentResult::data_point(
	true, true, "" );//TP
struct ExperimentResult::data_point* dp33 = new ExperimentResult::data_point(
	true, true, "" );//TP

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

ExperimentResult *er2 = new ExperimentResult(1,1);
er2->datum.data_points.push_back( dp02 );
er2->datum.data_points.push_back( dp12 );
er2->datum.data_points.push_back( dp22 );
er2->datum.data_points.push_back( dp32 );

ExperimentResult *er3 = new ExperimentResult(2,2);
er3->datum.data_points.push_back( dp03 );
er3->datum.data_points.push_back( dp13 );
er3->datum.data_points.push_back( dp23 );
er3->datum.data_points.push_back( dp33 );

ExperimentResultSet input_set(
	"Unit_Test2", //TODO make this remove spaces or take it out of naming
	"NFV",
	10,
	"wildpath",
	"datapath",
	20
	);//handles memory destruction for all of the above
input_set.push_back( er3 );//fpr 1
input_set.push_back( er );//fpr 0
input_set.push_back( er2 );//fpr 1

ExperimentAnalyzer ea;
ea.save_roc_script_data_points( input_set, "my_output" );
ea.save_experiment_results_data( input_set, "my_output"  );

}


//TEST save functions
//a. by hand
//b. by BOOST ostream test.

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_save_experiment_results_data )
{

//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct ExperimentResult::data_point* dp0 = new ExperimentResult::data_point(
	false, false, "" );//predicted, actual
struct ExperimentResult::data_point* dp1 = new ExperimentResult::data_point(
	false, false, "" );
struct ExperimentResult::data_point* dp2 = new ExperimentResult::data_point(
	false, false, "" );
struct ExperimentResult::data_point* dp3 = new ExperimentResult::data_point(
	false, false, "" );
//predicted, actual

ExperimentResult *er = new ExperimentResult(0,0);
er->datum.data_points.push_back( dp0 );
er->datum.data_points.push_back( dp1 );
er->datum.data_points.push_back( dp2 );
er->datum.data_points.push_back( dp3 );

struct ExperimentResult::data_point* dp02 = new ExperimentResult::data_point(
	true, true, "" );//predicted, actual
struct ExperimentResult::data_point* dp12 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp22 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp32 = new ExperimentResult::data_point(
	true, true, "" );
//predicted, actual

ExperimentResult *er2 = new ExperimentResult(1,1);
er2->datum.data_points.push_back( dp02 );
er2->datum.data_points.push_back( dp12 );
er2->datum.data_points.push_back( dp22 );
er2->datum.data_points.push_back( dp32 );

struct ExperimentResult::data_point* dp03 = new ExperimentResult::data_point(
	true, false, "" );//predicted, actual
struct ExperimentResult::data_point* dp13 = new ExperimentResult::data_point(
	false, true, "" );					  
struct ExperimentResult::data_point* dp23 = new ExperimentResult::data_point(
	true, true, "" );
struct ExperimentResult::data_point* dp33 = new ExperimentResult::data_point(
	false, false, "" );
//predicted, actual

ExperimentResult *er3 = new ExperimentResult(1,0);
er3->datum.data_points.push_back( dp03 );
er3->datum.data_points.push_back( dp13 );
er3->datum.data_points.push_back( dp23 );
er3->datum.data_points.push_back( dp33 );

ExperimentResultSet result_set( 
	"Unit_Test", 
	"NFV",
	10,
	"wildseqpath",
	"datapath",
	4	
	);

result_set.push_back( er );
result_set.push_back( er2 );
result_set.push_back( er3 );

ExperimentAnalyzer ea;
ea.save_experiment_results_data( result_set, "unit_test_output"  );

}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_get_experiment_ids_prediction )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	size_t seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Prediction_Experiment",
		seed
		);	

	Options options;//defaults

	ExperimentResult* result = new ExperimentResult(
		options.log_cost, options.log_gamma
		);

	ValidationExperiment val_exp;

	val_exp.run_experiment( 
	f.input_train_set,
	f.input_test_set,
	options,
	options.log_cost,
	options.log_gamma,
  	result
	);

	int k = f.input_test_set.size();
	for( int i =0 ; i < f.input_test_set.size(); i++ )
	{
		std::string s = f.input_test_set.at(i)->get_id();
	}

	ExperimentAnalyzer ea;
	std::vector<std::string> isolates  = ea.get_ids( result );

	BOOST_CHECK_EQUAL( isolates.size(), 2 );
	BOOST_CHECK_EQUAL( isolates.at(0), "CA2634" );
	BOOST_CHECK_EQUAL( isolates.at(1), "CA3872" );

	delete result;
}


BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_get_experiment_ids_model_selection )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	size_t seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Prediction_Experiment",
		seed
		);	


	CrossValidationExperiment cross_val_exp;

	Options o;//defaults

	//run experiment through grid search
	f.machine.parameter_search(
		f.cross_val_exp,
		f.input_train_set,
		o,
		1,//f.log_cost_low,
		1,//f.log_cost_high,
		1,//f.log_cost_increment,
		-5,//f.log_gamma_low,
		3,//f.log_gamma_high,
		2,//f.log_gamma_increment,
		f.result_set
		);

	int k = f.input_test_set.size();
	for( int i =0 ; i < f.input_train_set.size(); i++ )
	{
		std::string s = f.input_train_set.at(i)->get_id();
	}

	ExperimentAnalyzer ea;
	std::vector<std::string> isolates  = ea.get_ids( f.result_set.at(0) );

	BOOST_CHECK_EQUAL( isolates.size(), 4 );
	BOOST_CHECK_EQUAL( isolates.at(0), "CA3809" );
	BOOST_CHECK_EQUAL( isolates.at(1), "CA3833" );
	BOOST_CHECK_EQUAL( isolates.at(2), "CA4371" );
	BOOST_CHECK_EQUAL( isolates.at(3), "CA4390" );


	//NOW, let's confirm that 2nd cross validation experiment used same isolates as first
	std::vector<std::string> isolates2  = ea.get_ids( f.result_set.at(1) );

	BOOST_CHECK_EQUAL( isolates2.size(), 4 );
	BOOST_CHECK_EQUAL( isolates2.at(0), "CA3809" );
	BOOST_CHECK_EQUAL( isolates2.at(1), "CA3833" );
	BOOST_CHECK_EQUAL( isolates2.at(2), "CA4371" );
	BOOST_CHECK_EQUAL( isolates2.at(3), "CA4390" );
}

//eyeball verification
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_ids )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	size_t seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest_Fixture f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Prediction_Experiment",
		seed
		);	


	CrossValidationExperiment cross_val_exp;

	Options o;//defaults

	//run experiment through grid search
	f.machine.parameter_search(
		f.cross_val_exp,
		f.input_train_set,
		o,
		1,//f.log_cost_low,
		1,//f.log_cost_high,
		1,//f.log_cost_increment,
		-5,//f.log_gamma_low,
		3,//f.log_gamma_high,
		2,//f.log_gamma_increment,
		f.result_set
		);

	ExperimentAnalyzer ea;
	ea.save_model_selection_ids( f.result_set.at(0), "unit_test_expanalyzer" );

	//Output file should contain these values:
	//CA3809
	//CA3833
	//CA4371
	//CA4390
}

BOOST_AUTO_TEST_SUITE_END();

#endif//defined  EXPERIMENTANALYZER_TEST || defined TEST_ALL
#endif //EXPERIMENTANALYZERTEST_HPP
