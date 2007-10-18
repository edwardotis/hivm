#ifndef PREPROCWORKUNITTEST_HPP
#define PREPROCWORKUNITTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

#if defined  PREPROCWORKUNIT_TEST || defined TEST_ALL

#include "../PreProcWorkUnit.hpp"

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

#endif //#if defined  PREPROCWORKUNIT_TEST || defined TEST_ALL

#if defined  EXPERIMENTRESULT_TEST || defined TEST_ALL

#include "../ExperimentResult.hpp"

BOOST_AUTO_TEST_SUITE( ExperimentResultTests );

BOOST_AUTO_TEST_CASE( ExperimentResultTest_add_data )
{
	ExperimentResult *er = new ExperimentResult( 1, 1 );
	er->add_data( true, false, "ID1" );
	er->add_data( false, false, "ID2" );

	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->id, "ID1" );
	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->actual_susceptibility, false );

	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->id, "ID2" );
	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->actual_susceptibility, false );

	//struct data_point_set results = er->get_data();
	//results.data_points

	delete er;
}

BOOST_AUTO_TEST_CASE( ExperimentResultSetTest_add_result )
{
	ExperimentResult *er = new ExperimentResult( 1, 2 );
	er->add_data( true, false, "ID1" );
	er->add_data( false, false, "ID2" );

	ExperimentResult *er2 = new ExperimentResult( 3, 4 );
	er2->add_data( false, true, "ID3" );
	er2->add_data( true, true, "ID4" );

	ExperimentResultSet result_set;
	result_set.add( er );
	result_set.add( er2 );

	//first result
	BOOST_CHECK_EQUAL( result_set.front()->log_cost, 1 );
	BOOST_CHECK_EQUAL( result_set.front()->log_gamma, 2 );

	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->id, "ID1" );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->actual_susceptibility, false );

	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->id, "ID2" );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->actual_susceptibility, false );

	//second result
	BOOST_CHECK_EQUAL( result_set.back()->log_cost, 3 );
	BOOST_CHECK_EQUAL( result_set.back()->log_gamma, 4 );

	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.front()->id, "ID3" );
	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.front()->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.front()->actual_susceptibility, true );

	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.back()->id, "ID4" );
	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.back()->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( result_set.back()->data.data_points.back()->actual_susceptibility, true );
	
	//delete er, er2;
}


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
er3->data.data_points.push_back( dp03 );
er3->data.data_points.push_back( dp13 );
er3->data.data_points.push_back( dp23 );
er3->data.data_points.push_back( dp33 );

ExperimentResult *er_copy = new ExperimentResult( *er3 );//copy constructor

BOOST_CHECK_EQUAL( er3->log_cost, er_copy->log_cost );
BOOST_CHECK_EQUAL( er3->log_gamma, er_copy->log_gamma );

for( size_t i=0; i < er3->data.data_points.size(); i++)
{
	BOOST_CHECK_EQUAL( 
		er3->data.data_points.at(i)->predicted_susceptibility,
		er_copy->data.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		er3->data.data_points.at(i)->actual_susceptibility,
		er_copy->data.data_points.at(i)->actual_susceptibility
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

for( size_t i=0; i < result_set.front()->data.data_points.size(); i++)
{
	BOOST_CHECK_EQUAL( 
		result_set.front()->data.data_points.at(i)->predicted_susceptibility,
		result_set_copy.front()->data.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.front()->data.data_points.at(i)->actual_susceptibility,
		result_set_copy.front()->data.data_points.at(i)->actual_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.back()->data.data_points.at(i)->predicted_susceptibility,
		result_set_copy.back()->data.data_points.at(i)->predicted_susceptibility
		);

	BOOST_CHECK_EQUAL( 
		result_set.back()->data.data_points.at(i)->actual_susceptibility,
		result_set_copy.back()->data.data_points.at(i)->actual_susceptibility
		);
}

//delete er3;
//delete er_copy;

}//check memory leaks here

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  EXPERIMENTRESULT_TEST || defined TEST_ALL
#endif //PREPROCWORKUNITTEST_HPP
