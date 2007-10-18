
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/find_if.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/find_if.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/aux_/test.hpp>

#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_same.hpp>

typedef vector<int,char,long,short,char,long,double,float,char>::type types;
typedef begin<types>::type first_;

MPL_TEST_CASE()
{
    typedef find_if< types, boost::is_float<_> >::type iter;
    MPL_ASSERT(( is_same< iter::type, double > ));
    MPL_ASSERT_RELATION( (mpl::distance<first_,iter>::value), ==, 6 );
}

MPL_TEST_CASE()
{
    typedef find_if< types, boost::is_same<_,long> >::type iter;
    MPL_ASSERT(( is_same< iter::type, long > ));
    MPL_ASSERT_RELATION( (mpl::distance<first_,iter>::value), ==, 2 );
}

MPL_TEST_CASE()
{
    typedef find_if< types, boost::is_same<_,void> >::type iter;
    MPL_ASSERT(( is_same< iter, end<types>::type > ));
    MPL_ASSERT_RELATION( (mpl::distance<first_,iter>::value), ==, size<types>::value );
}
