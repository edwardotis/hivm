/**
Repository of definitions
*/

//Run full regression tests using every possible test available
//except for ones listed under ISOLATION TESTS
//#define TEST_ALL

//Run unit tests that take a long time in various test files
//#define LONG_TESTS	

//Individual unit test files to run
////
//#define BOOST_TEST
//#define FILEIO_TEST
//#define STL_TEST
//#define PREPROCESSOR_TEST
//#define DPL_TEST
//#define SVMWORKUNIT_TEST
//#define BIOPROCESSOR_TEST
#define LIBSVMADAPTER_TEST
//#define SELFTESTEXPERIMENTTEST_TEST
//#define SVMMACHINE_TEST//param search is medium long
//#define EXPERIMENTANALYZER_TEST
//#define PREPROCWORKUNIT_TEST //includes ExperimentResultTest
//#define CROSSVALIDATIONEXPERIMENTTEST_TEST
//#define VALIDATIONEXPERIMENTTEST_TEST
//#define OPTIONS_TEST

//not a unit test. just a hack before putting in
//cmd line parser
//has seeded shuffling experiments that belong in
//CrossValidationExp_Test, if we need to start shuffling
//crossval folds. Right now, there is no shuffling of this
//style used.
//#define EXPERIMENTTESTS_TEST



//ISOLATION TESTS
//Unit tests that must run in isolation
//These are not activated by TEST_ALL
//#define LOG_TEST

/**

Separate my auto test suites into long and short ones?


Or possibly this setup:
#define BOOST_TESTS
#define PREPROCESSOR_TEST


and then in each Test file.
For instance, PreProcessorTest.cpp

#include "Definitions.hpp"
#if defined  PREPROCESSOR_TEST || defined TEST_ALL

...
	#if defined  LONG_TESTS || defined TEST_ALL
		a long running test
	#endif

#endif

So, I can run regression tests (TEST_ALL) at night,
when this gets really long to run.
awesome.


*/

