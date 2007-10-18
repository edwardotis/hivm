/**
PURPOSE:
this class if for me to evaluate hivm's feedback to the user sa it runs.
Based on 2 main use cases, model-selection and model-validation.
And 2 minor cases, 
model-selection with --all-dataset option
model-validation with --test-dataset option
*/

#ifndef UITEST_HPP
#define UITEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  UI_TEST_UNATTENDED || defined TEST_ALL

#include "../SvmMachine.hpp"
#include "../Options.hpp"

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct UiTestUnattended_Fixture
{
	//default ctor
	UiTestUnattended_Fixture(){}

	//dtor
	~UiTestUnattended_Fixture(){}
};

//suite purpose: eyeball test that output is informative and correct for user
BOOST_AUTO_TEST_SUITE( UiTestsUnattended );

//model validation with --test-dataset option
BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_validation_test_dataset_option )
{
//hivm --purpose mode-selection --drug NFV
		
	int ac = 9;
	char* av[ 9 ];
	av[0] = "hivm";
	av[1] = "--drug";
	av[2] = "NFV";
	av[3] = "--hivdb-file";
	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv";
	av[5] = "--purpose";
	av[6] = "model-validation";
	av[7] = "--test-dataset";
	av[8] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}
//test defaults.
//BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_selection_base_case )
//{
////hivm --purpose mode-selection --drug NFV
//		
//	int ac = 5;
//	char* av[ 5 ];
//	av[0] = "hivm";
//	av[1] = "--drug";
//	av[2] = "NFV";
//	av[3] = "--hivdb-file";
//	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
//
//	Options o( ac, av );
//	SvmMachine machine;
//	machine.run( o.purpose, o );
//}
//
////test decimal grid search.
//BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_selection_decimal_grid_search )
//{
////hivm --purpose mode-selection --drug NFV
//		
//	int ac = 17;
//	char* av[ 17 ];
//	av[0] = "hivm";
//	av[1] = "--drug";
//	av[2] = "NFV";
//	av[3] = "--hivdb-file";
//	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
//	av[5] = "--lg-cost-low";
//	av[6] = "-12";
//	av[7] = "--lg-cost-high";
//	av[8] = "12";
//	av[9] = "--lg-cost-inc";
//	av[10] = ".5";
//	av[11] = "--lg-gamma-low";
//	av[12] = "2";
//	av[13] = "--lg-gamma-high";
//	av[14] = "4";
//	av[15] = "--lg-gamma-inc";
//	av[16] = "1";
//
//	Options o( ac, av );
//	SvmMachine machine;
//	machine.run( o.purpose, o );
//}

////test multiple thresholds
//BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_selection_multiple_thresholds )
//{
////hivm --purpose mode-selection --drug NFV
//		
//	int ac = 21;
//	char* av[ 21 ];
//	av[0] = "hivm";
//	av[1] = "--drug";
//	av[2] = "NFV";
//	av[3] = "--hivdb-file";
//	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
//	av[5] = "--lg-cost-low";
//	av[6] = "12";
//	av[7] = "--lg-cost-high";
//	av[8] = "12";
//	av[9] = "--lg-cost-inc";
//	av[10] = ".5";
//	av[11] = "--lg-gamma-low";
//	av[12] = "4";
//	av[13] = "--lg-gamma-high";
//	av[14] = "4";
//	av[15] = "--lg-gamma-inc";
//	av[16] = "1";
//	av[17] = "--thresholds";
//	av[18] = "2";
//	av[19] = "--thresholds";
//	av[20] = "10";
//
//	Options o( ac, av );
//	SvmMachine machine;
//	machine.run( o.purpose, o );
//}

//test defaults model validation
//BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_validation_base_case )
//{
////hivm --purpose mode-selection --drug NFV
//		
//	int ac = 7;
//	char* av[ 7 ];
//	av[0] = "hivm";
//	av[1] = "--drug";
//	av[2] = "NFV";
//	av[3] = "--hivdb-file";
//	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
//	av[5] = "--purpose";
//	av[6] = "model-validation";
//
//	Options o( ac, av );
//	SvmMachine machine;
//	machine.run( o.purpose, o );
//}

//ok, now the 2 minor cases.

//model selection with --all-dataset option
BOOST_AUTO_TEST_CASE( UiTestsUnattended_model_selection_all_dataset_option )
{
//hivm --purpose mode-selection --drug NFV --all-dataset 1
		
	int ac = 19;
	char* av[ 19 ];
	av[0] = "hivm";
	av[1] = "--drug";
	av[2] = "NFV";
	av[3] = "--hivdb-file";
	av[4] = "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv";
	av[5] = "--lg-cost-low";
	av[6] = "12";
	av[7] = "--lg-cost-high";
	av[8] = "13";
	av[9] = "--lg-cost-inc";
	av[10] = ".5";
	av[11] = "--lg-gamma-low";
	av[12] = "4";
	av[13] = "--lg-gamma-high";
	av[14] = "5";
	av[15] = "--lg-gamma-inc";
	av[16] = "1";
	av[17] = "--all-dataset";
	av[18] = "1";

	Options o( ac, av );
	SvmMachine machine;
	machine.run( o.purpose, o );
}



BOOST_AUTO_TEST_SUITE_END();//UiTestsUnattended

#endif //#if defined  UI_TEST || defined TEST_ALL
#endif//UITEST_HPP