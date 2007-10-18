
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/front.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/front.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/aux_/test.hpp>

template< typename Seq, int value > struct front_test
{
    typedef typename front<Seq>::type t;
    MPL_ASSERT_RELATION( t::value, ==, value );
};

MPL_TEST_CASE()
{
    front_test< range_c<int,1,10>, 1 >();
    front_test< range_c<int,2,10>, 2 >();
    front_test< range_c<int,-1,0>, -1 >();
}
