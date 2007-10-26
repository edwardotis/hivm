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
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values

BOOST_AUTO_TEST_SUITE( ExperimentResultTests );

BOOST_AUTO_TEST_CASE( ExperimentResultTest_add_data )
{
	std::vector<double> empty;
	ExperimentResult *er = new ExperimentResult( 1, 1 );
	er->add_data( true, false, "ID1", empty );//null if not doing probability
	er->add_data( false, false, "ID2", empty );//null if not doing probability

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

BOOST_AUTO_TEST_CASE( ExperimentResultTest_add_data_prob_destructor )
{
	std::vector<double> probs;
	probs.push_back( 1 );
	probs.push_back( 2 );

	ExperimentResult *er = new ExperimentResult( 1, 1 );
	er->add_data( true, false, "ID1", probs );//null if not doing probability
	er->add_data( false, false, "ID2", probs );//null if not doing probability

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

BOOST_AUTO_TEST_CASE( ExperimentResultTest_add_data_prob_copy_constructor )
{
	std::vector<double> prob1, prob2;
	prob1.push_back( .3 );
	prob1.push_back( .7 );
	prob2.push_back( .6 );
	prob2.push_back( .4 );	


	ExperimentResult *er = new ExperimentResult( 1, 1 );
	er->add_data( true, false, "ID1", prob1 );//null if not doing probability
	er->add_data( false, false, "ID2", prob2 );//null if not doing probability

	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->id, "ID1" );
	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( er->data.data_points.at(0)->actual_susceptibility, false );

	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->id, "ID2" );
	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( er->data.data_points.at(1)->actual_susceptibility, false );

	//copy constructor
	ExperimentResult *er2 = new ExperimentResult( *er );

	BOOST_CHECK_EQUAL( er2->data.data_points.at(0)->id, "ID1" );
	BOOST_CHECK_EQUAL( er2->data.data_points.at(0)->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( er2->data.data_points.at(0)->actual_susceptibility, false );

	std::vector<double> probs_out1 = er2->data.data_points.front()->probabilities;
	BOOST_CHECK_CLOSE( probs_out1.front(), .3, 0.001 );
	BOOST_CHECK_CLOSE( probs_out1.back(), .7, 0.001 );

	BOOST_CHECK_EQUAL( er2->data.data_points.at(1)->id, "ID2" );
	BOOST_CHECK_EQUAL( er2->data.data_points.at(1)->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( er2->data.data_points.at(1)->actual_susceptibility, false );

	std::vector<double> probs_out2 = er2->data.data_points.back()->probabilities;
	BOOST_CHECK_CLOSE( probs_out2.front(), .6, 0.001 );
	BOOST_CHECK_CLOSE( probs_out2.back(), .4, 0.001 );


	delete er; //deletes same memory 2x
	delete er2;
}

//todo switch to class model for unlimited number of classes instead of the binary model based on boolean.
BOOST_AUTO_TEST_CASE( ExperimentResultTest_add_data_probability )
{
	std::vector<double> probs_in1;
	probs_in1.push_back( .6 );//which class is the first one in my model? look it up.
	probs_in1.push_back( .4 );

	std::vector<double> probs_in2;
	probs_in2.push_back( .35 );
	probs_in2.push_back( .75 );

	ExperimentResult *er = new ExperimentResult( 1, 1 );
	er->add_data( true, false, "ID1",  probs_in1 );//null if not doing probability
	er->add_data( false, false, "ID2", probs_in2 );

	std::vector<double> probs_out1 = er->data.data_points.at(0)->probabilities;
	BOOST_CHECK_EQUAL( probs_out1.front(), .6 );
	BOOST_CHECK_EQUAL( probs_out1.back(), .4 );

	std::vector<double> probs_out2 = er->data.data_points.at(1)->probabilities;
	BOOST_CHECK_EQUAL( probs_out2.front(), .35 );
	BOOST_CHECK_EQUAL( probs_out2.back(), .75 );

	delete er;
}

BOOST_AUTO_TEST_CASE( ExperimentResultSetTest_add_result )
{
	std::vector<double> empty;
	ExperimentResult *er = new ExperimentResult( 1, 2 );
	er->add_data( true, false, "ID1", empty );
	er->add_data( false, false, "ID2", empty  );

	ExperimentResult *er2 = new ExperimentResult( 3, 4 );
	er2->add_data( false, true, "ID3", empty  );
	er2->add_data( true, true, "ID4", empty  );

	ExperimentResultSet result_set;
	result_set.add( er );
	result_set.add( er2 );

	//first result
	BOOST_CHECK_EQUAL( result_set.front()->lg_cost, 1 );
	BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, 2 );

	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->id, "ID1" );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->predicted_susceptibility, true );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.front()->actual_susceptibility, false );

	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->id, "ID2" );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->predicted_susceptibility, false );
	BOOST_CHECK_EQUAL( result_set.front()->data.data_points.back()->actual_susceptibility, false );

	//second result
	BOOST_CHECK_EQUAL( result_set.back()->lg_cost, 3 );
	BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, 4 );

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
	double lg_cost = 1;
	double lg_gamma = 2;
	std::vector<double> empty;

struct ExperimentResult::data_point* dp03 = new ExperimentResult::data_point(
	true, false, "", empty  );//predicted, actual
struct ExperimentResult::data_point* dp13 = new ExperimentResult::data_point(
	false, true, "", empty   );
struct ExperimentResult::data_point* dp23 = new ExperimentResult::data_point(
	true, true, "", empty   );
struct ExperimentResult::data_point* dp33 = new ExperimentResult::data_point(
	false, false, "", empty   );
//predicted, actual

ExperimentResult *er3 = new ExperimentResult( lg_cost,lg_gamma );
er3->data.data_points.push_back( dp03 );
er3->data.data_points.push_back( dp13 );
er3->data.data_points.push_back( dp23 );
er3->data.data_points.push_back( dp33 );

ExperimentResult *er_copy = new ExperimentResult( *er3 );//copy constructor

BOOST_CHECK_EQUAL( er3->lg_cost, er_copy->lg_cost );
BOOST_CHECK_EQUAL( er3->lg_gamma, er_copy->lg_gamma );

for( int i=0; i < er3->data.data_points.size(); i++)
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

BOOST_CHECK_EQUAL( result_set.front()->lg_cost, result_set_copy.front()->lg_cost );
BOOST_CHECK_EQUAL( result_set.front()->lg_gamma, result_set_copy.front()->lg_gamma );
BOOST_CHECK_EQUAL( result_set.back()->lg_cost, result_set_copy.back()->lg_cost );
BOOST_CHECK_EQUAL( result_set.back()->lg_gamma, result_set_copy.back()->lg_gamma );


BOOST_CHECK_EQUAL( result_set.wild_seq_path, result_set_copy.wild_seq_path );;
BOOST_CHECK_EQUAL( result_set.experiment_type, result_set_copy.experiment_type );
BOOST_CHECK_EQUAL( result_set.drug, result_set_copy.drug );
BOOST_CHECK_EQUAL( result_set.fold_threshold, result_set_copy.fold_threshold );
BOOST_CHECK_EQUAL( result_set.test_data_path, result_set_copy.test_data_path );
BOOST_CHECK_EQUAL( result_set.num_training_seqs, result_set_copy.num_training_seqs );
BOOST_CHECK_EQUAL( result_set.seed, result_set_copy.seed );


//do back()

for( int i=0; i < result_set.front()->data.data_points.size(); i++)
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