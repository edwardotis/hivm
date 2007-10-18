#ifndef STLTESTS_HPP
#define STLTESTS_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  STL_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <algorithm> //stl generic algorithms
#include <functional> //bind
#include <iostream>//string
#include <vector>
#include <boost/bind.hpp>

BOOST_AUTO_TEST_SUITE( stl_tests );

BOOST_AUTO_TEST_CASE( bool_test )
{
	if( false )
	{
		BOOST_CHECK_EQUAL( false, true );
	}

	if( 0 )//if false
	{
		BOOST_CHECK_EQUAL( 0, true );
	}

	if( true )
	{
		BOOST_CHECK_EQUAL( true, true );
	}

	if( 1 )//if true
	{
		BOOST_CHECK_EQUAL( 1, true );
	}

	//TODO switch these all to predicates <notequalto>
	if( -1 )//evaluates to true
	{
		//BOOST_CHECK_EQUAL( -1, true );//doesn't eval to true == true
	}

	bool b_true = true;
	bool b_false = false;
	
	BOOST_CHECK_EQUAL( b_true, 1 );
	//BOOST_CHECK_EQUAL( b_true, 0 );
	//BOOST_CHECK_EQUAL( b_true, -1 );

//	BOOST_CHECK_EQUAL( b_false, 1 );
	BOOST_CHECK_EQUAL( b_false, 0 );
/*	BOOST_CHECK_EQUAL( b_false, -1 )*/;

}

BOOST_AUTO_TEST_CASE( stl_tests_equal )
{
	std::string str1 = "hell\noh";
	std::string str2;

	//std::equal<char>(str.begin(), str.begin(), '\n' );//won't compile
	//std::equal(str.begin(), str.begin(), '\n' );//won't compile

	BOOST_CHECK( std::equal( str1.begin(), str1.begin(), "BAD_DATA" ) );

}

BOOST_AUTO_TEST_CASE( stl_tests_bind2nd_less )
{
	//std::string str = "hell\noh";
	std::vector<int> v( 5, 3 );

	int result = std::count_if( 
		v.begin(),
		v.end(),
		std::bind2nd( std::less<int>(), 5)
		);

	BOOST_CHECK_EQUAL( result, 5);
}

BOOST_AUTO_TEST_CASE( stl_tests_bind2nd_equal_to_char )
{
	std::string str = "hello\nworld\nhave\na nice day";

	int result = std::count_if( 
		str.begin(),
		str.end(),
		std::bind2nd( std::equal_to<char>(), '\n')
		);

	BOOST_CHECK_EQUAL( result, 3);
}

BOOST_AUTO_TEST_CASE( stl_tests_bind2nd_equal_to )
{
	// Relational Function Objects

	//size 5, with 5 3's initialized
	std::vector<int> v( 5, 3 );

	int result = std::count_if( 
		v.begin(),
		v.end(),
		std::bind2nd( 
			std::equal_to<int>(), 3
			)
		);

	BOOST_CHECK_EQUAL( result, 5 );
}

BOOST_AUTO_TEST_CASE( stl_tests_bind2nd_about_com_example )
{
	using namespace std;

   int ia1[5] = {1,2,3,4,5};
    vector<int> v1(ia1, ia1 + 5);

    vector<bool> v3(5), v4(5), v5(5);

//     Using "unary" version of transform

 //    compares values in v1 < 10 (second argument is bound)
    transform(
		v1.begin(), 
		v1.end(), 
		v4.begin(), 
		bind2nd(
			less<int>(),10
			) 
		);

	BOOST_CHECK_EQUAL( v4.front(), true);
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  STL_TESTS || defined TEST_ALL
#endif //STLTESTS_HPP
