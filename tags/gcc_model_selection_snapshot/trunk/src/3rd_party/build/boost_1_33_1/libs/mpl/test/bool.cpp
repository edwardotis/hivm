
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/bool.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/test.hpp>

#include <cassert>

#define BOOL_TEST(c) \
    { MPL_ASSERT(( is_same< bool_<c>::value_type, bool > )); } \
    { MPL_ASSERT(( is_same< bool_<c>, c##_ > )); } \
    { MPL_ASSERT(( is_same< bool_<c>::type, bool_<c> > )); } \
    { MPL_ASSERT_RELATION( bool_<c>::value, ==, c ); } \
    assert( bool_<c>() == c ); \
/**/

MPL_TEST_CASE()
{
    BOOL_TEST(true)
    BOOL_TEST(false)
}
