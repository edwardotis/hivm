#ifndef PREPROCWORKUNITTEST_HPP
#define PREPROCWORKUNITTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  PREPROCWORKUNIT_TEST || defined TEST_ALL

#include "../PreProcWorkUnit.hpp"
#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct PreProcWorkUnit_Fixture
{
	//ctor
    PreProcWorkUnit_Fixture(){}

	//dtor
    ~PreProcWorkUnit_Fixture(){}

};

BOOST_AUTO_TEST_SUITE( PreProcWorkUnitTests );

BOOST_AUTO_TEST_CASE( PreProcWorkUnitTest_Msg )
{

	BOOST_MESSAGE( "This class is too simple for tests." );
}

BOOST_AUTO_TEST_SUITE_END();

#include "../ExperimentResult.hpp"
BOOST_AUTO_TEST_SUITE( ExperimentResultTests );

BOOST_AUTO_TEST_CASE( ExperimentResultTest_copy_constructor )
{
	double log_cost = 1;
	double log_gamma = 2;

struct ExperimentResult::data_point* dp03 = new ExperimentResult::data_point(
	true, false, "" );//predicted, actual
struct ExperimentResult::data_point* dp13 = new ExperimentResult::data_point(
	false, true, ""  );
struct ExperimentResult::data_point* dp23 = new ExperimentResult::data_point(
	true, true, ""  );
struct ExperimentResult::data_point* dp33 = new ExperimentResult::data_point(
	false, false, ""  );
//predicted, actual

ExperimentResult *er3 = new ExperimentResult( log_cost,log_gamma );
er3->datum.data_points.push_back( dp03 );
er3->datum.data_points.push_back( dp13 );
er3->datum.data_points.push_back( dp23 );
er3->datum.data_points.push_back( dp33 );

ExperimentResult *er_copy = new ExperimentResult( *er3 );//copy constructor

BOOST_CHECK_EQUAL( er3->log_cost, er_copy->log_cost );
BOOST_CHECK_EQUAL( er3->log_gamma, er_copy->log_gamma );

for( size_t i=0; i < er3->datum.data_points.size(); i++)
{
	BOOST_CHECK_EQUAL( 
		er3->datum.data_points.at(i)->predicted_susceptibility,
		er_copy->datum.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		er3->datum.data_points.at(i)->actual_susceptibility,
		er_copy->datum.data_points.at(i)->actual_susceptibility
		);
}

//now let's do copy constructor for ExperimentResultSet
ExperimentResultSet result_set(
	"Cross_Val", //TODO make this remove spaces or take it out of naming
	"NFV",
	10,
	"wildpath",
	"datapath",
	20
	);
result_set.set_seed( 42 );

result_set.push_back( er3 );
result_set.push_back( er_copy );

ExperimentResultSet result_set_copy( result_set );//copy constructor

BOOST_CHECK_EQUAL( result_set.front()->log_cost, result_set_copy.front()->log_cost );
BOOST_CHECK_EQUAL( result_set.front()->log_gamma, result_set_copy.front()->log_gamma );
BOOST_CHECK_EQUAL( result_set.back()->log_cost, result_set_copy.back()->log_cost );
BOOST_CHECK_EQUAL( result_set.back()->log_gamma, result_set_copy.back()->log_gamma );


BOOST_CHECK_EQUAL( result_set.wild_seq_path, result_set_copy.wild_seq_path );;
BOOST_CHECK_EQUAL( result_set.experiment_type, result_set_copy.experiment_type );
BOOST_CHECK_EQUAL( result_set.drug, result_set_copy.drug );
BOOST_CHECK_EQUAL( result_set.fold_threshold, result_set_copy.fold_threshold );
BOOST_CHECK_EQUAL( result_set.test_data_path, result_set_copy.test_data_path );
BOOST_CHECK_EQUAL( result_set.num_training_seqs, result_set_copy.num_training_seqs );
BOOST_CHECK_EQUAL( result_set.seed, result_set_copy.seed );


//do back()

for( size_t i=0; i < result_set.front()->datum.data_points.size(); i++)
{
	BOOST_CHECK_EQUAL( 
		result_set.front()->datum.data_points.at(i)->predicted_susceptibility,
		result_set_copy.front()->datum.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.front()->datum.data_points.at(i)->actual_susceptibility,
		result_set_copy.front()->datum.data_points.at(i)->actual_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.back()->datum.data_points.at(i)->predicted_susceptibility,
		result_set_copy.back()->datum.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.back()->datum.data_points.at(i)->actual_susceptibility,
		result_set_copy.back()->datum.data_points.at(i)->actual_susceptibility
		);
}

//delete er3;
//delete er_copy;

}//check memory leaks here

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  PREPROCWORKUNIT_TEST || defined TEST_ALL
#endif //PREPROCWORKUNITTEST_HPP
