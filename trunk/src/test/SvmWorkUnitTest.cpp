#ifndef SVMWORKUNITTEST_HPP
#define SVMWORKUNITTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  SVMWORKUNIT_TEST || defined TEST_ALL

#include "../SvmWorkUnit.hpp"
#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct SvmWorkUnit_Fixture
{
	//ctor
    SvmWorkUnit_Fixture(){}

	//dtor
    ~SvmWorkUnit_Fixture(){}

};

BOOST_AUTO_TEST_SUITE( SvmWorkUnitTests );

//search for memory leaks
//BOOST_AUTO_TEST_CASE( SvmWorkUnit_Destructor_single )
//{
//	SvmWorkUnit* wu = new SvmWorkUnit();
//	delete wu;
//}

//search for memory leaks
BOOST_AUTO_TEST_CASE( SvmWorkUnit_Destructor_set )
{
	{
		SvmWUSet wu_set( "my id", false );

		for(int i = 0; i < 3; i++ )
		{
			//SvmWorkUnit* wu = new SvmWorkUnit();
			wu_set.push_back( i );
		}
	}
}

////search for memory leaks
BOOST_AUTO_TEST_CASE( SvmWorkUnit_Destructor_matrix )
{
	SvmWUMatrix wu_matrix;

	for( int row=0; row < 4; row++ )
	{
		SvmWUSet* wu_set = new SvmWUSet( "my id", false );

		for(int i = 0; i < 3; i++ )
		{
			//SvmWorkUnit* wu = new SvmWorkUnit();
			wu_set->push_back( i );
		}

		wu_matrix.push_back( wu_set );
	}
}

BOOST_AUTO_TEST_CASE( SvmWorkUnit_other_aspects )
{
//	//each matrix has size, name i.e. training set, predictees set)
//	//each row (i.e. set) has ID and isResistant
//	//each matrix(j,k) has bio_proc_score. i.e. local alignment score
//	//matrix should always be nxn

	SvmWUMatrix wu_matrix;

	for( int row=0; row < 4; row++ )
	{
		SvmWUSet* wu_set = new SvmWUSet( "Test ID", false );

		for(int i = 0; i < 3; i++ )
		{
			//SvmWorkUnit* wu = new SvmWorkUnit( 42 );
			wu_set->push_back( i );
		}

		wu_matrix.push_back( wu_set );
	}
	
	//should SvmWU not exist? Just use doubles?
	//and row as ADT instead of SvmWUSet?
	//will I ever need to store more than a double?
	//can I use [][] notation if I have pointers?
	//how much does it matter? take a shower. then come ack to it.

	//ok, this will work as my notation.
	BOOST_CHECK_EQUAL( 0, wu_matrix[0]->at(0) );
	BOOST_CHECK_EQUAL( 1, wu_matrix[0]->at(1) );
	BOOST_CHECK_EQUAL( 2, wu_matrix[0]->at(2) );
	BOOST_CHECK_EQUAL( 0, wu_matrix[1]->at(0) );
	BOOST_CHECK_EQUAL( 0, wu_matrix[2]->at(0) );
	BOOST_CHECK_EQUAL( "Test ID", wu_matrix[0]->get_id() );
	BOOST_CHECK_EQUAL( false, wu_matrix[0]->known_susceptibility() );
	
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  SVMWORKUNIT_TEST || defined TEST_ALL
#endif //SVMWORKUNITTEST_HPP
