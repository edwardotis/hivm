//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: test_fp_comparisons.cpp,v $
//
//  Version     : $Revision: 1.1 $
//
//  Description : tests floating point comparison algorithms
// ***************************************************************************

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/floating_point_comparison.hpp>

// Boost
#include <boost/mpl/list.hpp>
#include <boost/bind.hpp>

// STL
#include <functional>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::test_tools;

bool not_func( bool arg ) { return !arg; }

//____________________________________________________________________________//

typedef boost::mpl::list<float,double,long double> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_BOOST_CHECK_CLOSE, FPT, test_types )
{
#define CHECK_CLOSE( first, second, e )     \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_CHECK_CLOSE( fp1, fp2, epsilon ); \
/**/

#define CHECK_NOT_CLOSE( first, second, e ) \
    fp1     = static_cast<FPT>(first);      \
    fp2     = static_cast<FPT>(second);     \
    epsilon = static_cast<FPT>(e);          \
                                            \
    BOOST_CHECK_PREDICATE( bind(not_func, bind(check_is_close, _1, _2, _3)), (fp1)(fp2)(epsilon) ); \
/**/

    FPT fp1, fp2, epsilon;

    CHECK_CLOSE( 1, 1, 0 );

    CHECK_NOT_CLOSE( 0, 1e-20, 1e-5 );
    CHECK_NOT_CLOSE( 0, 1e-30, 1e-5 );
    CHECK_NOT_CLOSE( 0, -1e-10, 0.1 );
    CHECK_NOT_CLOSE( 0.123456, 0.123457, 1e-4 );

    CHECK_CLOSE( 0.123456, 0.123457, 1e-3 );

    CHECK_NOT_CLOSE( 0.123456, -0.123457, 1e-3 );

    CHECK_CLOSE( 1.23456e28, 1.23457e28, 1e-3 );

    CHECK_CLOSE( 1.23456e-10, 1.23457e-10, 1e-3 );
    CHECK_NOT_CLOSE( 1.111e-10, 1.112e-10, 0.08999 );
    CHECK_CLOSE( 1.112e-10, 1.111e-10, 0.1 );

    CHECK_CLOSE( 1, 1.0001, 1.1e-2 );
    CHECK_CLOSE( 1.0002, 1.0001, 1.1e-2 );

    CHECK_NOT_CLOSE( 1, 1.0002, 1.1e-2 );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_CHECK_SMALL )
{
    BOOST_CHECK_SMALL( 1e-6, 1e-5 );
    BOOST_CHECK_SMALL( -1e-6, 1e-5 );

    BOOST_CHECK_PREDICATE( bind(not_func, bind(check_is_small, _1, _2 )), (1e-6)(1e-7) );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_close_at_tolerance )
{
    double fp1     = 1.00000001;
    double fp2     = 1.00000002;
    double epsilon = 1e-6;

    close_at_tolerance<double> pred( epsilon, FPC_WEAK );
    BOOST_CHECK_PREDICATE( pred, (fp1)(fp2) );

    BOOST_CHECK_PREDICATE( bind(not_func, bind(check_is_close, _1, _2, _3)), (fp1)(fp2)(epsilon) );

    fp1     = 1.23456e-10;
    fp2     = 1.23457e-10;
    epsilon = 8.1e-4;

    BOOST_CHECK_PREDICATE( close_at_tolerance<double>( epsilon, FPC_WEAK ), (fp1)(fp2) );
    BOOST_CHECK_PREDICATE( bind(not_func, bind(close_at_tolerance<double>( epsilon ), _1, _2)), (fp1)(fp2) );
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//
//  $Log: test_fp_comparisons.cpp,v $
//  Revision 1.1  2006/08/07 05:44:28  ed
//  load boost files for making libraries for each architecture
//
//  Revision 1.22  2005/05/29 08:54:04  rogeeff
//  remove format testing
//
//  Revision 1.17  2005/02/20 08:28:34  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

// EOF
