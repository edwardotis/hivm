#ifndef EXPERIMENTANALYZERTEST_HPP
#define EXPERIMENTANALYZERTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  EXPERIMENTANALYZER_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/lexical_cast.hpp> //safe numerical to string casts
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values

#include "../ExperimentAnalyzer.hpp"
#include "../ExperimentResult.hpp"
#include "../PreProcessor.hpp"
//#include "../SelfTestExperiment.hpp"
#include "../SvmMachine.hpp"
#include "../Log.hpp"
#include "../FunctionObjects.cpp"//functor for random integer in random_shuffle
#include "../CrossValidationExperiment.hpp"
#include "../ValidationExperiment.hpp"

class ExperimentAnalyzerTest
{

public:
	
	//default ctor
	ExperimentAnalyzerTest(){}

	//ctor
	ExperimentAnalyzerTest(
		std::string test_data_path,
		std::string wild_seq, 
		std::string drug, 
		double threshold,
		std::string experiment_type,
		int seed
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
		//result_set.set_seed( seed );

		//result_set.set_params(
		//	experiment_type,
		//	drug,
		//	threshold,
		//	wild_seq,
		//	test_data_path,
		//	input_train_set.size()
		//	);

		Log::append( "ExperimentAnalyzerTest__Fixture input_train_set.size = " +  boost::lexical_cast<std::string>( input_train_set.size() )  );
	}

	//dtor
	~ExperimentAnalyzerTest(){}

	void save_prediction_detailed_results()
	{
		//goal. setup so that I call save_prediction_detailed_results directly.
		//not through analyze
		//set variables
		//std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
		//std::string wild_seq_path = "../data/shared/PI_wild.seq";
		//std::string drug = "NFV";
		//double threshold = 10;//should be median of the file
		//bool self_test = false;
		//int seed = 42;

			int ac = 33;
			char* av[ 33 ];
			av[0] = "hivm";

			av[1] = "-x";
			av[2] = "1";
			av[3] = "-y";
			av[4] = "2";
			av[5] = "-z";
			av[6] = "3";

			av[7] = "--lg-gamma-low";
			av[8] = "4";
			av[9] = "-m";
			av[10] = "5";
			av[11] = "-n";
			av[12] = "6";

			av[13] = "-d";
			av[14] = "NFV";

			//av[15] = "-t";
			//av[16] = "4";

			av[15] = "-t";
			av[16] = "10";

			av[17] = "-w";
			av[18] = "../data/shared/PI_wild.seq";

			av[19] = "-e";
			av[20] = "clinical";

			av[21] = "-s";
			av[22] = "42";

			av[23] = "-o";
			av[24] = "../results/Ed";

			av[25] = "-p";
			av[26] = "model-validation";

			av[27] = "-f";
			av[28] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv";

			av[29] = "-c";
			av[30] = "7.5";

			av[31] = "-g";
			av[32] = "-5.5";
			
		{
			Options options( ac, av );
			options.probability = 1;//use probability

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			ExperimentResult* result = new ExperimentResult();

			ValidationExperiment val_exp;

			val_exp.run_experiment( 
				input_train_set,
				input_test_set,
				options,
  				result
				);

			ExperimentAnalyzer analyzer;
			std::vector<double> probabilities;

			for( int i = 0; i < result->size(); i++ )
			{
				probabilities = result->at(i)->probabilities;

				//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
				BOOST_CHECK_EQUAL( probabilities.size(), 2 );

				//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
				// so a -1 means that the values were never set properly by libsvm.
				for(int j=0; j< probabilities.size(); j++ )
				{
					BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( j ) ) );
				}

				double sucep = probabilities.at( 1 );
				double resist = probabilities.at( 0 );
				int size = result->size();
				//std::cerr << size << '\n';

				if( result->at(i)->predicted_susceptibility == true )//suceptible
				{
					BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 1 ) ) (probabilities.at( 0 ) ) );
				}

				if( result->at(i)->predicted_susceptibility  == false )//resistant
				{
					BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 0 ) ) ( probabilities.at( 1 ) ) );
				}

				//test determine probability
				std::string probstr = analyzer.determine_probability( result, i );
				double prob = boost::lexical_cast<double>( probstr );
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( prob ) ( 0.5 ) );
			}

			//eyeball
			analyzer.save_prediction_detailed_results( result, "save_prediction_detailed_results_w_probability" );

			delete result;
		}

		{
			Options options( ac, av );
			options.probability = 0;//do NOT use probability

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			ExperimentResult* result = new ExperimentResult();

			ValidationExperiment val_exp;

			val_exp.run_experiment( 
				input_train_set,
				input_test_set,
				options,
  				result
				);

			ExperimentAnalyzer analyzer;
			analyzer.save_prediction_detailed_results( result, "save_prediction_detailed_results_WITHOUT_probability" );

			delete result;
		}
			//eyeball verification
			//todo Use Boost Test Tools stream function to match output stream
			//to a text file that I created
			
	}

	void save_prediction_detailed_results_fulldataset()
	{
		//goal. setup so that I call save_prediction_detailed_results directly.
		//not through analyze
		//set variables
		//std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
		//std::string wild_seq_path = "../data/shared/PI_wild.seq";
		//std::string drug = "NFV";
		//double threshold = 10;//should be median of the file
		//bool self_test = false;
		//int seed = 42;

			int ac = 33;
			char* av[ 33 ];
			av[0] = "hivm";

			av[1] = "-x";
			av[2] = "1";
			av[3] = "-y";
			av[4] = "2";
			av[5] = "-z";
			av[6] = "3";

			av[7] = "--lg-gamma-low";
			av[8] = "4";
			av[9] = "-m";
			av[10] = "5";
			av[11] = "-n";
			av[12] = "6";

			av[13] = "-d";
			av[14] = "IDV";

			//av[15] = "-t";
			//av[16] = "4";

			av[15] = "-t";
			av[16] = "10";

			av[17] = "-w";
			av[18] = "../data/shared/PI_wild.seq";

			av[19] = "-e";
			av[20] = "clinical";

			av[21] = "-s";
			av[22] = "42";

			av[23] = "-o";
			av[24] = "../results/Ed";

			av[25] = "-p";
			av[26] = "model-validation";

			av[27] = "-f";
			av[28] = "../data/shared/PR_2006-05-25_v2.0.tsv";

			av[29] = "-c";
			av[30] = "1.5";

			av[31] = "-g";
			av[32] = "-1";
			
		{
			Options options( ac, av );
			options.probability = 1;//use probability

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			ExperimentResult* result = new ExperimentResult();

			ValidationExperiment val_exp;

			val_exp.run_experiment( 
				input_train_set,
				input_test_set,
				options,
  				result
				);

			ExperimentAnalyzer analyzer;
			std::vector<double> probabilities;

			for( int i = 0; i < result->size(); i++ )
			{
				probabilities = result->at(i)->probabilities;

				//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
				BOOST_CHECK_EQUAL( probabilities.size(), 2 );

				//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
				// so a -1 means that the values were never set properly by libsvm.
				for(int j=0; j< probabilities.size(); j++ )
				{
					BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( j ) ) );
				}

				double sucep = probabilities.at( 1 );
				double resist = probabilities.at( 0 );
				int size = result->size();

				if( result->at(i)->predicted_susceptibility == true )//suceptible
				{
					BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 1 ) ) (probabilities.at( 0 ) ) );
				}

				if( result->at(i)->predicted_susceptibility  == false )//resistant
				{
					BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 0 ) ) ( probabilities.at( 1 ) ) );
				}

				//test determine probability
				std::string probstr = analyzer.determine_probability( result, i );
				double prob = boost::lexical_cast<double>( probstr );
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( prob ) ( 0.5 ) );
			}

			//eyeball
			analyzer.save_prediction_detailed_results( result, "save_prediction_detailed_results_w_probability_long" );

			delete result;
		}


			//eyeball verification
			//todo Use Boost Test Tools stream function to match output stream
			//to a text file that I created
		
	}

	void prune()
	{

		ExperimentAnalyzerTest f;
		//make a result set with some duplicate fpr's and diff tpr's.
		//see if prune works on it.

	//TPR = 2/(2) = 1 TP/( TP + FN )
	//FPR = FP/(TN + FP) =
	//      0/2 = 0;
	struct data_point* dp0 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp1 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp2 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp3 = new data_point(
		false, false, "", f.empty );//TN

	//TPR = 1/(1) = 1
	//FPR = 1/2   = .5 FP/(TN + FP)
	struct data_point* dp02 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp12 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp22 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp32 = new data_point(
		true, false, "", f.empty );//FP

	//TPR = 2/(3) = .66666
	//FPR = 1/1   = 1 FP/(TN + FP)
	struct data_point* dp03 = new data_point(
		false, true, "", f.empty );//FN
	struct data_point* dp13 = new data_point(
		true, false, "", f.empty );//FP
	struct data_point* dp23 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp33 = new data_point(
		true, true, "", f.empty );//TP

	//TPR = 2/(3) = .66666
	//FPR = 1/1   = 1 FP/(TN + FP)
	struct data_point* dp04 = new data_point(
		false, true, "", f.empty );//FN
	struct data_point* dp14 = new data_point(
		true, false, "", f.empty );//FP
	struct data_point* dp24 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp34 = new data_point(
		true, true, "", f.empty );//TP

	//TPR = 2/(2) = 1 TP/( TP + FN )
	//FPR = FP/(TN + FP) =
	//      0/2 = 0;
	struct data_point* dp05 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp15 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp25 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp35 = new data_point(
		false, false, "", f.empty );//TN

	//TPR = 1/(1) = 1
	//FPR = 1/2   = .5 FP/(TN + FP)
	struct data_point* dp06 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp16 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp26 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp36 = new data_point(
		true, false, "", f.empty );//FP

	//TPR = 1/(4) = .25 TP/( TP + FN )
	//FPR = UNDEFINED = -1   = FP/(TN + FP)
	struct data_point* dp07 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp17 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp27 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp37 = new data_point(
		true, true, "", f.empty );//TP

	//TPR = UNDEFINED = -1 TP/( TP + FN )
	//FPR = 2/4  = 0.5 FP/(TN + FP)
	struct data_point* dp08 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp18 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp28 = new data_point(
		true, false, "", f.empty );//FP
	struct data_point* dp38 = new data_point(
		true, false, "", f.empty );//FP

	ExperimentResult *er = new ExperimentResult(1,1);
	er->push_back( dp0 );
	er->push_back( dp1 );
	er->push_back( dp2 );
	er->push_back( dp3 );

	ExperimentResult *er2 = new ExperimentResult(2,2);
	er2->push_back( dp02 );
	er2->push_back( dp12 );
	er2->push_back( dp22 );
	er2->push_back( dp32 );

	ExperimentResult *er3 = new ExperimentResult(3,3);
	er3->push_back( dp03 );
	er3->push_back( dp13 );
	er3->push_back( dp23 );
	er3->push_back( dp33 );

	ExperimentResult *er4 = new ExperimentResult(4,4);
	er4->push_back( dp04 );
	er4->push_back( dp14 );
	er4->push_back( dp24 );
	er4->push_back( dp34 );

	ExperimentResult *er5 = new ExperimentResult(5,5);
	er5->push_back( dp05 );
	er5->push_back( dp15 );
	er5->push_back( dp25 );
	er5->push_back( dp35 );

	ExperimentResult *er6 = new ExperimentResult(6,6);
	er6->push_back( dp06 );
	er6->push_back( dp16 );
	er6->push_back( dp26 );
	er6->push_back( dp36 );

	ExperimentResult *er7 = new ExperimentResult(7,7);
	er7->push_back( dp07 );
	er7->push_back( dp17 );
	er7->push_back( dp27 );
	er7->push_back( dp37 );

	ExperimentResult *er8 = new ExperimentResult(8,8);
	er8->push_back( dp08 );
	er8->push_back( dp18 );
	er8->push_back( dp28 );
	er8->push_back( dp38 );


	ExperimentResultSet input_set;//handles memory destruction for all of the above
	input_set.push_back( er5 );//fpr 0
	input_set.push_back( er3 );//fpr 1
	input_set.push_back( er );//fpr 0
	input_set.push_back( er6 );//fpr .5
	input_set.push_back( er4 );//fpr 1
	input_set.push_back( er2 );//fpr .5
	input_set.push_back( er7 );//fpr -1
	input_set.push_back( er8 );//fpr 1

	ExperimentAnalyzer ea;
	//
	////verify unsorted fpr's
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(0) ), 0 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(1) ), 1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(2) ), 0 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(3) ), .5 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(4) ), 1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(5) ), .5 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(6) ), -1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(7) ), .5 );
	BOOST_CHECK_EQUAL( ea.tpr( input_set.at(7) ), -1 );


	ExperimentResultSet pruned_set;
	ea.prune( input_set, pruned_set );

	//check that input was not sorted by fpr
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(0) ), 0 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(1) ), 1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(2) ), 0 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(3) ), .5 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(4) ), 1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(5) ), .5 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(6) ), -1 );
	BOOST_CHECK_EQUAL( ea.fpr( input_set.at(7) ), .5 );
	BOOST_CHECK_EQUAL( ea.tpr( input_set.at(7) ), -1 );

	//verify pruning of output set.
	BOOST_CHECK_EQUAL( pruned_set.size(), 3 );

	BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(0) ),  0 );
	BOOST_CHECK_EQUAL( ea.tpr( pruned_set.at(0) ), 1 );

	BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(1) ), .5 );
	BOOST_CHECK_EQUAL( ea.tpr( pruned_set.at(1) ), 1 );

	BOOST_CHECK_EQUAL( ea.fpr( pruned_set.at(2) ), 1 );
	BOOST_CHECK_CLOSE( ea.tpr( pruned_set.at(2) ), .666666, .01 );

	ea.save_gnuplot_script( pruned_set, "prune2", "NFV" ); 
}


	void prune2()
	{
	//set variables
		std::string test_data_path = "../data/ExperimentTests/PR_2006-05-25_v2.0_NFV50.tsv";
		std::string wild_seq_path = "../data/shared/PI_wild.seq";
		std::string drug = "NFV";
		double threshold = 10;//should be median of the file
		bool self_test = true;
		int seed = 42;

		//initialize for cross val exp
		ExperimentAnalyzerTest f( 
			test_data_path, 
			wild_seq_path,
			drug,
			threshold,
			"Cross_Val_Experiment",
			seed
			);	

		Options o;//defaults
		//turn off probability b/c previous results were generated
		//without probability
		o.probability = 0;


		//run experiment through grid search
		f.machine.parameter_search(
			f.cross_val_exp,
			f.input_train_set,
			o,
			1,//f.lg_cost_low,
			1,//f.lg_cost_high,
			1,//f.lg_cost_inc,
			-5,//f.lg_gamma_low,
			3,//f.lg_gamma_high,
			2,//f.lg_gamma_inc,
			f.result_set
			);

	/** 

	KNOWN RESULTS on NFV 50 BEFORE PRUNING (non-probability version)
	UPDATED b/c HIVM shuffle is switched to portable BOOST random number generator now.
	lg_cost	 lg_gamma	 TPR	 FPR
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

	ExperimentResultSet pruned_result_set;
	ea.prune( f.result_set, pruned_result_set );//prune

	BOOST_CHECK_EQUAL( 4, pruned_result_set.size() );


	BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(0) ), 0.5000, 0.1 );//
		
	BOOST_CHECK_EQUAL( pruned_result_set.at(0)->lg_cost, 1 );
	BOOST_CHECK_EQUAL( pruned_result_set.at(0)->lg_gamma, -1 );

	BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(1) ), 0.6429, 0.1 );//
	BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(1) ), 0.6875, 0.1 );//
	BOOST_CHECK_EQUAL( pruned_result_set.at(1)->lg_cost, 1 );
	BOOST_CHECK_EQUAL( pruned_result_set.at(1)->lg_gamma, -3 );

	BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(2) ), 0.7857, 0.1 );//
	BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(2) ), 0.5625, 0.1 );//
	BOOST_CHECK_EQUAL( pruned_result_set.at(2)->lg_cost, 1 );
	BOOST_CHECK_EQUAL( pruned_result_set.at(2)->lg_gamma, 3 );

	BOOST_CHECK_CLOSE( ea.fpr( pruned_result_set.at(3) ), 1.0000, 0.1 );//
	BOOST_CHECK_CLOSE( ea.tpr( pruned_result_set.at(3) ), 0.6250, 0.1 );//
	BOOST_CHECK_EQUAL( pruned_result_set.at(3)->lg_cost, 1 );
	BOOST_CHECK_EQUAL( pruned_result_set.at(3)->lg_gamma, -5 );

}


	//Terrible test. You have 3 different FPR's, so you can't compare jack.
	void save_roc_data()
	{
		ExperimentAnalyzerTest f;
	//TPR = 2/(2) = 1 TP/( TP + FN )
	//FPR = 0/2   = 0
	struct data_point* dp0 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp1 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp2 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp3 = new data_point(
		false, false, "", f.empty );//TN

	//TPR = 1/(1) = 1
	//FPR = 1/2   = 1 FP/(TN + FN)
	struct data_point* dp02 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp12 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp22 = new data_point(
		false, false, "", f.empty );//TN
	struct data_point* dp32 = new data_point(
		true, false, "", f.empty );//FP

	//TPR = 2/(3) = .66666
	//FPR = 1/1   = 1
	struct data_point* dp03 = new data_point(
		false, true, "", f.empty );//FN
	struct data_point* dp13 = new data_point(
		true, false, "", f.empty );//FP
	struct data_point* dp23 = new data_point(
		true, true, "", f.empty );//TP
	struct data_point* dp33 = new data_point(
		true, true, "", f.empty );//TP

	ExperimentResult *er = new ExperimentResult(0,0);
	er->push_back( dp0 );
	er->push_back( dp1 );
	er->push_back( dp2 );
	er->push_back( dp3 );

	ExperimentResult *er2 = new ExperimentResult(1,1);
	er2->push_back( dp02 );
	er2->push_back( dp12 );
	er2->push_back( dp22 );
	er2->push_back( dp32 );

	ExperimentResult *er3 = new ExperimentResult(2,2);
	er3->push_back( dp03 );
	er3->push_back( dp13 );
	er3->push_back( dp23 );
	er3->push_back( dp33 );

	ExperimentResultSet input_set;//handles memory destruction for all of the above
	input_set.push_back( er3 );//fpr 1
	input_set.push_back( er );//fpr 0
	input_set.push_back( er2 );//fpr 1

	ExperimentAnalyzer ea;
	ea.save_roc_script_data_points( input_set, "my_output" );
	ea.save_experiment_results_data( input_set, "my_output"  );
	}


	void stats_save_experiment_results_data()
	{
		ExperimentAnalyzerTest f;

	//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
	struct data_point* dp0 = new data_point(
		false, false, "", f.empty );//predicted, actual
	struct data_point* dp1 = new data_point(
		false, false, "", f.empty );
	struct data_point* dp2 = new data_point(
		false, false, "", f.empty );
	struct data_point* dp3 = new data_point(
		false, false, "", f.empty );
	//predicted, actual

	ExperimentResult *er = new ExperimentResult(0,0);
	er->push_back( dp0 );
	er->push_back( dp1 );
	er->push_back( dp2 );
	er->push_back( dp3 );

	struct data_point* dp02 = new data_point(
		true, true, "", f.empty );//predicted, actual
	struct data_point* dp12 = new data_point(
		true, true, "", f.empty );
	struct data_point* dp22 = new data_point(
		true, true, "", f.empty );
	struct data_point* dp32 = new data_point(
		true, true, "", f.empty );
	//predicted, actual

	ExperimentResult *er2 = new ExperimentResult(1,1);
	er2->push_back( dp02 );
	er2->push_back( dp12 );
	er2->push_back( dp22 );
	er2->push_back( dp32 );

	struct data_point* dp03 = new data_point(
		true, false, "", f.empty );//predicted, actual
	struct data_point* dp13 = new data_point(
		false, true, "", f.empty );					  
	struct data_point* dp23 = new data_point(
		true, true, "", f.empty );
	struct data_point* dp33 = new data_point(
		false, false, "", f.empty );
	//predicted, actual

	ExperimentResult *er3 = new ExperimentResult(1,0);
	er3->push_back( dp03 );
	er3->push_back( dp13 );
	er3->push_back( dp23 );
	er3->push_back( dp33 );

	ExperimentResultSet result_set;

	result_set.push_back( er );
	result_set.push_back( er2 );
	result_set.push_back( er3 );

	ExperimentAnalyzer ea;
	ea.save_experiment_results_data( result_set, "unit_test_output"  );

	}


void analyze_prediction_experiment()
{

			int ac = 33;
		char* av[ 33 ];
		av[0] = "hivm";

		av[1] = "-x";
		av[2] = "1";
		av[3] = "-y";
		av[4] = "2";
		av[5] = "-z";
		av[6] = "3";

		av[7] = "--lg-gamma-low";
		av[8] = "4";
		av[9] = "-m";
		av[10] = "5";
		av[11] = "-n";
		av[12] = "6";

		av[13] = "-d";
		av[14] = "NFV";

		//av[15] = "-t";
		//av[16] = "4";

		av[15] = "-t";
		av[16] = "10";

		av[17] = "-w";
		av[18] = "../data/shared/PI_wild.seq";

		av[19] = "-e";
		av[20] = "clinical";

		av[21] = "-s";
		av[22] = "42";

		av[23] = "-o";
		av[24] = "../results/Ed";

		av[25] = "-p";
		av[26] = "model-validation";

		av[27] = "-f";
		av[28] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";

		av[29] = "-c";
		av[30] = "7.5";

		av[31] = "-g";
		av[32] = "-5.5";

		{
			Options options( ac, av );
			options.probability = 1;//use probability
			options.output_prefix = "analyze_prediction_experiment_w_prob";

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			SvmMachine machine;
			machine.model_validate( options );//calls analyze implicitly

		}

		{
			Options options( ac, av );
			options.probability = 0;//don't use probability
			options.output_prefix = "analyze_prediction_experiment_withOUT_prob";

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			SvmMachine machine;
			machine.model_validate( options );//calls analyze implicitly

		}
		
		//now make a long one with probability
		{
			Options options( ac, av );
			options.susceptibility_file = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV50.tsv";
			options.probability = 1;//use probability
			options.output_prefix = "analyze_prediction_experiment_with_prob_50";

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			SvmMachine machine;
			machine.model_validate( options );//calls analyze implicitly
		}

		//now make a long one without probability
		{
			Options options( ac, av );
			options.susceptibility_file = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV50.tsv";
			options.probability = 0;//use probability
			options.output_prefix = "analyze_prediction_experiment_withOUT_prob_50";

			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			PreProcessor p;
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

			SvmMachine machine;
			machine.model_validate( options );//calls analyze implicitly
		}

	//Output file should contain these values:
	//CA3809, true class, class predicted, probability, lg cost, lg gamma
	//CA3833
	//CA4371
	//CA4390
	
	//test without probability
	//test with probability 
	//Options o2;//defaults
	//o2.probability = 0;//use probability

	//f.machine.test( o2 );

	//Output file should contain these values:
	//CA3809, true class, class predicted, n/a, lg cost, lg gamma
	//CA3833
	//CA4371
	//CA4390

}
void get_experiment_ids_model_selection()
{

	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	int seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest f( 
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
		1,//f.lg_cost_low,
		1,//f.lg_cost_high,
		1,//f.lg_cost_inc,
		-5,//f.lg_gamma_low,
		3,//f.lg_gamma_high,
		2,//f.lg_gamma_inc,
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


void save_model_selection_ids()
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	int seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Model_Selection_Experiment",
		seed
		);	


	CrossValidationExperiment cross_val_exp;

	Options o;//defaults

	//run experiment through grid search
	f.machine.parameter_search(
		f.cross_val_exp,
		f.input_train_set,
		o,
		1,//f.lg_cost_low,
		1,//f.lg_cost_high,
		1,//f.lg_cost_inc,
		-5,//f.lg_gamma_low,
		3,//f.lg_gamma_high,
		2,//f.lg_gamma_inc,
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


public:
	PreProcessor p;
	PreProcWUSet input_train_set;
	PreProcWUSet input_test_set;
	SvmMachine machine;
	//SelfTestExperiment self_test_exp;
	CrossValidationExperiment cross_val_exp;
	ExperimentResultSet result_set;
	Options options;
	std::vector<double> empty;
};

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_compute_roc_auc )
{
	//first test. 2 data points that fill entire x-axis
	//(0,.5) and (1,1) == (x1,y1) and (x2,y2)
	//Area of Trapezoid = ((a+b)h )/2, where h = x2-x1 and
	//a = y1-0, b = y2-0
	//basically rotate the roc curve 90 degrees right.
	//A = ((.5+1)1)/2 = .75

	ExperimentAnalyzer ea;
	
	ExperimentAnalyzerTest fix;
	//make a result set with some duplicate fpr's and diff tpr's.
	//see if prune works on it.

	//TPR =  TP/( TP + FN ) = 1
	//FPR =  FP/(TN + FP) = 1
	struct data_point* dp0 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp1 = new data_point(
		true, false, "", fix.empty );//FP

	//TPR = TP/( TP + FN )= .5
	//FPR =  FP/(TN + FP) = 0
	struct data_point* dp02 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp12 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp22 = new data_point(
		false, true, "", fix.empty );//FN

	ExperimentResult *er = new ExperimentResult(1,1);
	er->push_back( dp0 );
	er->push_back( dp1 );

	ExperimentResult *er2 = new ExperimentResult(2,2);
	er2->push_back( dp02 );
	er2->push_back( dp12 );
	er2->push_back( dp22 );

	fix.result_set.push_back( er );// (1,1)
	fix.result_set.push_back( er2 );//(0, .5)

	bool use_extra_endpoints = true;
	bool dont_use_extra_endpoints = false;

	//(0,.5) and (1,1)
	double area1 = ea.compute_roc_auc( fix.result_set, dont_use_extra_endpoints );
	BOOST_CHECK_EQUAL( .75, area1 );

	double area2 = ea.compute_roc_auc( fix.result_set, use_extra_endpoints );
	BOOST_CHECK_EQUAL( 0.75, area2 );


}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_compute_roc_auc2 )
{
	//first test. 2 data points that fill entire x-axis
	//(0,.5) and (1,1) == (x1,y1) and (x2,y2)
	//Area of Trapezoid = ((a+b)h )/2, where h = x2-x1 and
	//a = y1-0, b = y2-0
	//basically rotate the roc curve 90 degrees right.
	//A = ((.5+1)1)/2 = .75

	ExperimentAnalyzer ea;
	
	ExperimentAnalyzerTest fix;
	//make a result set with some duplicate fpr's and diff tpr's.
	//see if prune works on it.

	//(.25,.5) (.5,.75)

	//TPR =  TP/( TP + FN ) = 1/(1+1) = .5
	//FPR =  FP/(TN + FP) = 1/(3+1) = .25
	struct data_point* dp1 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp2 = new data_point(
		false, true, "", fix.empty );//FN
	struct data_point* dp3 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp4 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp5 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp6 = new data_point(
		true, false, "", fix.empty );//FP

	//TPR =  TP/( TP + FN ) = 3/(3+1) = .75
	//FPR =  FP/(TN + FP) = 1/(1+1) = .5
	struct data_point* dp10 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp11 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp12 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp13 = new data_point(
		false, true, "", fix.empty );//FN
	struct data_point* dp14 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp15 = new data_point(
		true, false, "", fix.empty );//FP

	ExperimentResult *er = new ExperimentResult(1,1);
	er->push_back( dp1 );
	er->push_back( dp2 );
	er->push_back( dp3 );
	er->push_back( dp4 );
	er->push_back( dp5 );
	er->push_back( dp6 );


	ExperimentResult *er2 = new ExperimentResult(2,2);
	er2->push_back( dp10 );
	er2->push_back( dp11 );
	er2->push_back( dp12 );
	er2->push_back( dp13 );
	er2->push_back( dp14 );
	er2->push_back( dp15 );

	
	fix.result_set.push_back( er );// (.25,.5)
	fix.result_set.push_back( er2 );// (.5,.75)

	bool use_extra_endpoints = true;
	bool dont_use_extra_endpoints = false;

	// (.25,.5) (.5,.75)
	double area1 = ea.compute_roc_auc( fix.result_set, dont_use_extra_endpoints );
	BOOST_CHECK_EQUAL( 0.15625, area1 );

	double area2 = ea.compute_roc_auc( fix.result_set, use_extra_endpoints );
	BOOST_CHECK_EQUAL( 0.65625, area2 );
}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_compute_roc_auc3 )
{
	//first test. 2 data points that fill entire x-axis
	//(0,.5) and (1,1) == (x1,y1) and (x2,y2)
	//Area of Trapezoid = ((a+b)h )/2, where h = x2-x1 and
	//a = y1-0, b = y2-0
	//basically rotate the roc curve 90 degrees right.
	//A = ((.5+1)1)/2 = .75

	ExperimentAnalyzer ea;
	
	ExperimentAnalyzerTest fix;
	//make a result set with some duplicate fpr's and diff tpr's.
	//see if prune works on it.

	//(.25,.5) 

	//TPR =  TP/( TP + FN ) = 1/(1+1) = .5
	//FPR =  FP/(TN + FP) = 1/(3+1) = .25
	struct data_point* dp1 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp2 = new data_point(
		false, true, "", fix.empty );//FN
	struct data_point* dp3 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp4 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp5 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp6 = new data_point(
		true, false, "", fix.empty );//FP

	 //(.5,.75)
	//TPR =  TP/( TP + FN ) = 3/(3+1) = .75
	//FPR =  FP/(TN + FP) = 1/(1+1) = .5
	struct data_point* dp10 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp11 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp12 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp13 = new data_point(
		false, true, "", fix.empty );//FN
	struct data_point* dp14 = new data_point(
		false, false, "", fix.empty );//TN
	struct data_point* dp15 = new data_point(
		true, false, "", fix.empty );//FP

	// (.75,.8)
	//TPR =  TP/( TP + FN ) = 4/(4+1) = .8
	//FPR =  FP/(TN + FP) = 3/(1+3) = .75
	struct data_point* dp20 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp21 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp22 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp23 = new data_point(
		true, true, "", fix.empty );//TP
	struct data_point* dp24 = new data_point(
		false, true, "", fix.empty );//FN

	struct data_point* dp25 = new data_point(
		true, false, "", fix.empty );//FP
	struct data_point* dp26 = new data_point(
		true, false, "", fix.empty );//FP
	struct data_point* dp27 = new data_point(
		true, false, "", fix.empty );//FP
	struct data_point* dp28 = new data_point(
		false, false, "", fix.empty );//TN


	ExperimentResult *er = new ExperimentResult(1,1);
	er->push_back( dp1 );
	er->push_back( dp2 );
	er->push_back( dp3 );
	er->push_back( dp4 );
	er->push_back( dp5 );
	er->push_back( dp6 );


	ExperimentResult *er2 = new ExperimentResult(2,2);
	er2->push_back( dp10 );
	er2->push_back( dp11 );
	er2->push_back( dp12 );
	er2->push_back( dp13 );
	er2->push_back( dp14 );
	er2->push_back( dp15 );

	ExperimentResult *er3 = new ExperimentResult(3,3);
	er3->push_back( dp20 );
	er3->push_back( dp21 );
	er3->push_back( dp22 );
	er3->push_back( dp23 );
	er3->push_back( dp24 );
	er3->push_back( dp25 );
	er3->push_back( dp26 );
	er3->push_back( dp27 );
	er3->push_back( dp28 );
	
	fix.result_set.push_back( er );// (.25,.5)
	fix.result_set.push_back( er2 );// (.5,.75)
	fix.result_set.push_back( er3 );//  (.75,.8)

	bool use_extra_endpoints = true;
	bool dont_use_extra_endpoints = false;

	// (.25,.5) (.5,.75) (.75,.8)
	double area1 = ea.compute_roc_auc( fix.result_set, dont_use_extra_endpoints );
	BOOST_CHECK_EQUAL( 0.35, area1 );

	double area2 = ea.compute_roc_auc( fix.result_set, use_extra_endpoints );
	BOOST_CHECK_EQUAL( 0.6375, area2 );
}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_all_false )
{
	//test the stats functions
	//create an ExperimentResult here with simple
	//datapoint set. Do, the math, and make sure the
	//app matches on the stats.
ExperimentAnalyzerTest f;
	
//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct data_point* dp0 = new data_point(
	false, false, "", f.empty );//predicted, actual
struct data_point* dp1 = new data_point(
	false, false, "", f.empty );
struct data_point* dp2 = new data_point(
	false, false, "", f.empty );
struct data_point* dp3 = new data_point(
	false, false, "", f.empty );
//predicted, actual

ExperimentResult *er = new ExperimentResult(0,0);
er->push_back( dp0 );
er->push_back( dp1 );
er->push_back( dp2 );
er->push_back( dp3 );

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
ExperimentAnalyzerTest f;

//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct data_point* dp0 = new data_point(
	true, true, "", f.empty );//predicted, actual
struct data_point* dp1 = new data_point(
	true, true, "", f.empty );
struct data_point* dp2 = new data_point(
	true, true, "", f.empty );
struct data_point* dp3 = new data_point(
	true, true, "", f.empty );
//predicted, actual

ExperimentResult *er = new ExperimentResult(0,0);
er->push_back( dp0 );
er->push_back( dp1 );
er->push_back( dp2 );
er->push_back( dp3 );

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

ExperimentAnalyzerTest f;
//	data_point(	bool _predicted_susceptibility,	bool _actual_susceptibility)
struct data_point* dp0 = new data_point(
	true, false, "", f.empty );//predicted, actual
struct data_point* dp1 = new data_point(
	false, true, "", f.empty );
struct data_point* dp2 = new data_point(
	true, true, "", f.empty );
struct data_point* dp3 = new data_point(
	false, false, "", f.empty );
//predicted, actual
//1 of each

ExperimentResult *er = new ExperimentResult(0,0);
er->push_back( dp0 );
er->push_back( dp1 );
er->push_back( dp2 );
er->push_back( dp3 );

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
	ExperimentAnalyzerTest t;
	t.prune();	
}

#if defined LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_prune2 )
{
	ExperimentAnalyzerTest t;
	t.prune2();	
}


#endif //#if defined LONG_TESTS || defined TEST_ALL

//test by looking at file for now.
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_roc_data )
{
	ExperimentAnalyzerTest t;
	t.save_roc_data();	
}


//TEST save functions
//a. by hand
//b. by BOOST ostream test.

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_stats_save_experiment_results_data )
{
	ExperimentAnalyzerTest t;
	t.stats_save_experiment_results_data();	
}

BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_get_experiment_ids_prediction )
{
	//set variables
	std::string test_data_path = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	std::string wild_seq_path = "../data/shared/PI_wild.seq";
	std::string drug = "NFV";
	double threshold = 10;//should be median of the file
	bool self_test = false;
	int seed = 42;

	//initialize for cross val exp
	ExperimentAnalyzerTest f( 
		test_data_path, 
		wild_seq_path,
		drug,
		threshold,
		"Prediction_Experiment",
		seed
		);	

	Options options;//defaults

	ExperimentResult* result = new ExperimentResult(
		options.lg_cost, options.lg_gamma
		);

	ValidationExperiment val_exp;

	val_exp.run_experiment( 
	f.input_train_set,
	f.input_test_set,
	options,
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
	ExperimentAnalyzerTest t;
	t.get_experiment_ids_model_selection();	
}

//eyeball verification
//todo Use Boost Test Tools stream function to match output stream
//to a text file that I created
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_model_selection_ids )
{
	ExperimentAnalyzerTest t;
	t.save_model_selection_ids();	
}




//eyeball verification
//todo Use Boost Test Tools stream function to match output stream
//to a text file that I created
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_prediction_detailed_results )
{
	ExperimentAnalyzerTest t;
	t.save_prediction_detailed_results ();	
}


#if defined LONG_TESTS || defined TEST_ALL
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_save_prediction_detailed_results_fulldataset )
{
	ExperimentAnalyzerTest t;
	t.save_prediction_detailed_results_fulldataset();	
}
#endif //#if defined LONG_TESTS || defined TEST_ALL


//eyeball verification
//todo Use Boost Test Tools stream function to match output stream
////to a text file that I created
BOOST_AUTO_TEST_CASE( ExperimentAnalyzerTest_analyze_prediction_experiment )
{
	ExperimentAnalyzerTest t;
	t.analyze_prediction_experiment();	
}


BOOST_AUTO_TEST_SUITE_END();

#endif//defined  EXPERIMENTANALYZER_TEST || defined TEST_ALL
#endif //EXPERIMENTANALYZERTEST_HPP
