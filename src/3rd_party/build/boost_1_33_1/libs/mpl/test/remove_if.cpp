
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/remove_if.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/remove_if.hpp>

#include <boost/mpl/list/list10_c.hpp>
#include <boost/mpl/list/list10.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/is_float.hpp>

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef list10_c<int,0,1,2,3,4,5,6,7,8,9> numbers;
    typedef list5_c<int,4,3,2,1,0>::type answer;
    typedef remove_if<
          numbers
        , greater<_,int_<4> >
        , mpl::front_inserter< list0_c<int> >
        >::type result;

    MPL_ASSERT_RELATION( size<result>::value, ==, 5 );
    MPL_ASSERT(( equal<result,answer> ));
}

MPL_TEST_CASE()
{
    typedef list8<int,float,long,float,char,long,double,double> types;
    typedef list4<int,long,char,long>::type answer;
    typedef reverse_remove_if<
          types
        , is_float<_>
        , mpl::front_inserter< list0<> >
        >::type result;

    MPL_ASSERT_RELATION( size<result>::value, ==, 4 );
    MPL_ASSERT(( equal<result,answer> ));
}