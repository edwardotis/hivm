
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/size.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/size.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef list<> empty_list;
    typedef list<char,short,int,long> list;
    
    MPL_ASSERT_RELATION( size<empty_list>::value, ==, 0 );
    MPL_ASSERT_RELATION( size<list>::value, ==, 4 );
}
