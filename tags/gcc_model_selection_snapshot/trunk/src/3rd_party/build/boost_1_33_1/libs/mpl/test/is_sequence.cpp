
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/is_sequence.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/aux_/test.hpp>

template< typename T > struct std_vector
{
    T* begin();
};

MPL_TEST_CASE()
{
    MPL_ASSERT_NOT(( is_sequence< std_vector<int> > ));
    MPL_ASSERT_NOT(( is_sequence< int > ));
    MPL_ASSERT_NOT(( is_sequence< int& > ));
    MPL_ASSERT_NOT(( is_sequence< UDT > ));
    MPL_ASSERT_NOT(( is_sequence< UDT* > ));
    MPL_ASSERT(( is_sequence< range_c<int,0,0> > ));
    MPL_ASSERT(( is_sequence< list<> > ));
    MPL_ASSERT(( is_sequence< list<int> > ));
    MPL_ASSERT(( is_sequence< vector<> > ));
    MPL_ASSERT(( is_sequence< vector<int> > ));
}