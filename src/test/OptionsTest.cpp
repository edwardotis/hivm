#ifndef OPTIONSTEST_HPP
#define OPTIONSTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  OPTIONS_TEST || defined TEST_ALL

#include "../Options.hpp"
#include "../Log.hpp"
#include "../PreProcessor.hpp"

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

namespace po = boost::program_options;

struct OptionsTest_Fixture
{
	//ctor
	OptionsTest_Fixture(){}

	//dtor
	~OptionsTest_Fixture(){}
};

BOOST_AUTO_TEST_SUITE( OptionsTests );

//simple case
BOOST_AUTO_TEST_CASE( OptionsTest_first )
{	//do this guy first. simplest
	//hivm --operation test --drug NFV --thresholds 2 10 
	{
		int ac = 9;
		char* av[ 9 ];
		av[0] = "hivm";
		av[1] = "--purpose";
		av[2] = "prediction";
		av[3] = "--drug";
		av[4] = "NFV";
		av[5] = "--thresholds";
		av[6] = "2";
		av[7] = "--thresholds";
		av[8] = "10";

		Options o(ac, av);
		//o.parse_options( ac, av, vm );

		po::variables_map my_vm;
		my_vm = o.get_options();

		BOOST_CHECK_EQUAL( "prediction", my_vm["purpose"].as<std::string>() );
		BOOST_CHECK_EQUAL( "prediction", o.purpose );
		BOOST_CHECK_EQUAL( "NFV", my_vm["drug"].as<std::string>() );

		std::vector<double> v = my_vm["thresholds"].as<std::vector<double> >();
		BOOST_CHECK_EQUAL( 2, v.at(0) );
		BOOST_CHECK_EQUAL( 10, v.at(1) );
	}

	{
		int ac = 9;
		char* av[ 9 ];
		av[0] = "hivm";
		av[1] = "-p";
		av[2] = "test";
		av[3] = "-d";
		av[4] = "NFV";
		av[5] = "-t";
		av[6] = "2";
		av[7] = "-t";
		av[8] = "10";

		Options o(ac, av);
		//o.parse_options( ac, av, vm );

		po::variables_map my_vm;
		my_vm = o.get_options();

		BOOST_CHECK_EQUAL( "test", my_vm["purpose"].as<std::string>() );
		//BOOST_CHECK_EQUAL( "test", vm["o"].as<std::string>() );//DOESN'T WORK. bad cast
		
		BOOST_CHECK_EQUAL( "NFV", my_vm["drug"].as<std::string>() );

		std::vector<double> v = my_vm["thresholds"].as<std::vector<double> >();
		BOOST_CHECK_EQUAL( 2, v.at(0) );
		BOOST_CHECK_EQUAL( 10, v.at(1) );
	}
}

//test that the test options are set as expected
//BOOST_AUTO_TEST_CASE( OptionsTest_test )
//{
	//do this  first. simplest
	//hivm --operation test --drug NFV --thresholds 2 10 --wild-type-sequence "..data/shared/PI_wild.seq"
	//--hivdb-susceptibility-file PR_2006-05-25_v2.0.tsv --output-prefix mytest

	//int ac;
	//char* av[5];
//}


//test that the cross-validation options are set as expected
BOOST_AUTO_TEST_CASE( OptionsTest_cross_validation )
{
	{
		int ac = 37;
		char* av[ 37 ];
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

		av[17] = "-t";
		av[18] = "11";

		av[15] = "-t";
		av[16] = "4";

		av[19] = "-w";
		av[20] = "../wild";

		av[21] = "-e";
		av[22] = "lab";

		av[23] = "-s";
		av[24] = "13";

		av[25] = "-o";
		av[26] = "../results/Ed";

		av[27] = "-p";
		av[28] = "model-selection";

		av[29] = "-f";
		av[30] = "../suscep";

		av[31] = "-c";
		av[32] = "7.5";

		av[33] = "-g";
		av[34] = "8.5";

		av[35] = "--probability";
		av[36] = "1";

		Log l;//init the log.

		Options o(ac, av);
		//o.parse_options( , vm );

		po::variables_map my_vm;
		my_vm = o.get_options();

		BOOST_CHECK_EQUAL( o.seed, 13 );
		BOOST_CHECK_EQUAL( o.drug, "NFV" );
		BOOST_CHECK_EQUAL( o.thresholds.at(0), 4 );
		BOOST_CHECK_EQUAL( o.thresholds.at(1), 11 );
		BOOST_CHECK_EQUAL( o.wild_type_file, "../wild" );
		BOOST_CHECK_EQUAL( o.susceptibility_file, "../suscep" );
		BOOST_CHECK_EQUAL( o.purpose, "model-selection" );
		BOOST_CHECK_EQUAL( o.output_prefix, "../results/Ed" );
		BOOST_CHECK_EQUAL( o.susceptibility_type, "Lab" );

		BOOST_CHECK_EQUAL( o.lg_cost_low, 1 );
		BOOST_CHECK_EQUAL( o.lg_cost_high, 2 );
		BOOST_CHECK_EQUAL( o.lg_cost_inc, 3 );

		BOOST_CHECK_EQUAL( o.lg_gamma_low, 4 );
		BOOST_CHECK_EQUAL( o.lg_gamma_high, 5 );
		BOOST_CHECK_EQUAL( o.lg_gamma_inc, 6 );

		BOOST_CHECK_EQUAL( o.lg_cost, 7.5 );
		BOOST_CHECK_EQUAL( o.lg_gamma, 8.5 );

		BOOST_CHECK_EQUAL( o.probability, 1 );

		o.save_cmd_line_options();

	}
}

//test that the test options are set as expected
BOOST_AUTO_TEST_CASE( OptionsTest_model_validation )
{
	{
		int ac = 41;
		char* av[ 41 ];
		av[0] = "hivm";

		av[1] = "--lg-cost-low";
		av[2] = "1";
		av[3] = "--lg-cost-high";
		av[4] = "2";
		av[5] = "--lg-cost-inc";
		av[6] = "3";

		av[7] = "--lg-gamma-low";
		av[8] = "4";
		av[9] = "--lg-gamma-high";
		av[10] = "5";
		av[11] = "--lg-gamma-inc";
		av[12] = "6";

		av[13] = "--drug";
		av[14] = "NFV";

		av[17] = "--thresholds";
		av[18] = "11";

		av[15] = "--thresholds";
		av[16] = "4";

		av[19] = "--wild-type";
		av[20] = "../wild";

		av[21] = "--suscep-type";
		av[22] = "lab";

		av[23] = "--seed";
		av[24] = "13";

		av[25] = "--output";
		av[26] = "../results/OptionsTest_model_validation";

		av[27] = "--purpose";
		av[28] = "model-validation";

		av[29] = "--hivdb-file";
		av[30] = "../suscep";

		av[31] = "--lg-gamma-g";
		av[32] = "8.5";

		av[33] = "--lg-cost-c";
		av[34] = "7.5";

		av[35] = "--probability";
		av[36] = "0";

		av[37] = "--all-dataset";
		av[38] = "1";

		av[39] = "--test-dataset";
		av[40] = "../suscep/test_dataset.csv";

		Log l;//init the log.

		Options o(ac, av);
		//o.parse_options( , vm );

		po::variables_map my_vm;
		my_vm = o.get_options();

		BOOST_CHECK_EQUAL( o.seed, 13 );
		BOOST_CHECK_EQUAL( o.drug, "NFV" );
		BOOST_CHECK_EQUAL( o.thresholds.at(0), 4 );
		BOOST_CHECK_EQUAL( o.thresholds.at(1), 11 );
		BOOST_CHECK_EQUAL( o.wild_type_file, "../wild" );
		BOOST_CHECK_EQUAL( o.susceptibility_file, "../suscep" );
		BOOST_CHECK_EQUAL( o.purpose, "model-validation" );
		BOOST_CHECK_EQUAL( o.output_prefix, "../results/OptionsTest_model_validation" );
		BOOST_CHECK_EQUAL( o.susceptibility_type, "Lab" );

		BOOST_CHECK_EQUAL( o.lg_cost_low, 1 );
		BOOST_CHECK_EQUAL( o.lg_cost_high, 2 );
		BOOST_CHECK_EQUAL( o.lg_cost_inc, 3 );

		BOOST_CHECK_EQUAL( o.lg_gamma_low, 4 );
		BOOST_CHECK_EQUAL( o.lg_gamma_high, 5 );
		BOOST_CHECK_EQUAL( o.lg_gamma_inc, 6 );

		BOOST_CHECK_EQUAL( o.lg_cost, 7.5 );
		BOOST_CHECK_EQUAL( o.lg_gamma, 8.5 );

		BOOST_CHECK_EQUAL( o.probability, 0 );

		BOOST_CHECK_EQUAL( o.use_entire_susceptbility_file, 1 );
		BOOST_CHECK_EQUAL( o.susceptibility_file_test_dataset, "../suscep/test_dataset.csv" );

		o.save_cmd_line_options();

	}
}


//test ref to vm
BOOST_AUTO_TEST_CASE( OptionsTest_vm )
{
	int ac = 5;
	char* av[ 5 ];
	av[0] = "hivm";
	av[1] = "-x";
	av[2] = "3";
	av[3] = "-c";
	av[4] = "4";

	Options o(ac, av);

	po::variables_map my_vm;
	my_vm = o.get_options();

	BOOST_CHECK_EQUAL( 3, my_vm["lg-cost-low"].as<double>() );
	BOOST_CHECK_EQUAL( 4, o.lg_cost );

}

//Test two thresholds, but dumping medium group of input seq's. i.e. only high and low are kept
//TODO test me
//BOOST_AUTO_TEST_CASE( OptiounsTest_parseInputFiles_pr_small7highlow_two_thresholds_sort )
//{
//	
//		int ac = 37;
//		char* av[ 37 ];
//		av[0] = "hivm";
//
//		av[1] = "--lg-cost-low";
//		av[2] = "1";
//		av[3] = "--lg-cost-high";
//		av[4] = "2";
//		av[5] = "--lg-cost-inc";
//		av[6] = "3";
//
//		av[7] = "--lg-gamma-low";
//		av[8] = "4";
//		av[9] = "--lg-gamma-high";
//		av[10] = "5";
//		av[11] = "--lg-gamma-inc";
//		av[12] = "6";
//
//		av[13] = "--drug";
//		av[14] = "NFV";
//
//		av[17] = "--thresholds";//high
//		av[18] = "10";
//
//		av[15] = "--thresholds";//low, so out of order
//		av[16] = "2";
//
//		av[19] = "--wild-type";
//		av[20] = "../data/shared/PI_wild.seq";
//
//		av[21] = "--suscep-type";
//		av[22] = "all";
//
//		av[23] = "--seed";
//		av[24] = "42";
//
//		av[25] = "--output";
//		av[26] = "../results/Ed";
//
//		av[27] = "--purpose";
//		av[28] = "model-selection";
//
//		av[29] = "--hivdb-file";
//		av[30] = "../suscep";
//
//		av[31] = "--lg-gamma-g";
//		av[32] = "8.5";
//
//		av[33] = "--lg-cost-c";
//		av[34] = "7.5";
//
//		av[35] = "--probability";
//		av[36] = "0";
//				
//		
//		Options options(ac, av);
//
//	PreProcessor pre_proc;
//	PreProcWUSet wu_set;
//	//std::vector<double> thresholds;
//	//thresholds.push_back( 2 );//low threshold
//	//thresholds.push_back( 10);//high threshold
//
//
//	//using the selftest version of parseInputFiles
//	pre_proc.parseInputFiles(
//		"../data/PreProcessorTest/PR_2006-05-25_v2.0_small7_highlow.tsv",
//		"../data/shared/PI_wild.seq",
//		"NFV",
//		options.thresholds,
//		options,
//		wu_set
//		);
//
//	//*** Breakdown of file by hand based on 2 and 10 thresholds inclusive.
//	//no duplicates or >, < results.
//	//index, H = high, L = low
//	//0 header row
//	//1 H
//	//2 L
//	//3 H
//	//4 M
//	//5 H
//	//6 M
//	//7 L
//
//	//post preprocess indices
//	//0 H 
//	//1 L
//	//2 H
//	//3 H
//	//4 L
//
//	BOOST_CHECK_EQUAL( 5, wu_set.size() );//7 minus two medium resistant
//
//	BOOST_CHECK_EQUAL( "CA2634", wu_set[0]->get_id() );
//	BOOST_CHECK_EQUAL( false, wu_set[0]->known_susceptibility() );
//
//	BOOST_CHECK_EQUAL( "SD-10", wu_set[1]->get_id() );
//	BOOST_CHECK_EQUAL( true, wu_set[1]->known_susceptibility() );
//
//	BOOST_CHECK_EQUAL( "ALD-pt611", wu_set[2]->get_id() );
//	BOOST_CHECK_EQUAL( false, wu_set[2]->known_susceptibility() );
//
//	BOOST_CHECK_EQUAL( "V2031", wu_set[3]->get_id() );
//	BOOST_CHECK_EQUAL( false, wu_set[3]->known_susceptibility() );
//
//	BOOST_CHECK_EQUAL( "CA3872", wu_set[4]->get_id() );
//	BOOST_CHECK_EQUAL( true, wu_set[4]->known_susceptibility() );
//}



BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  OPTIONS_TEST || defined TEST_ALL
#endif //OPTIONSTEST_HPP
