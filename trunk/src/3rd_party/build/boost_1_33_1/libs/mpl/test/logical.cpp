
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/logical.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/logical.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/test.hpp>

struct unknown;

using mpl::true_;
using mpl::false_;

MPL_TEST_CASE()
{
    MPL_ASSERT(( mpl::and_< true_,true_ > ));
    MPL_ASSERT_NOT(( mpl::and_< false_,true_ > ));
    MPL_ASSERT_NOT(( mpl::and_< true_,false_ > ));
    MPL_ASSERT_NOT(( mpl::and_< false_,false_ > ));
    MPL_ASSERT_NOT(( mpl::and_< false_,unknown > ));
    MPL_ASSERT_NOT(( mpl::and_< false_,unknown,unknown > ));

    MPL_ASSERT(( mpl::or_< true_,true_ > ));
    MPL_ASSERT(( mpl::or_< false_,true_ > ));
    MPL_ASSERT(( mpl::or_< true_,false_ > ));
    MPL_ASSERT_NOT(( mpl::or_< false_,false_ > ));
    MPL_ASSERT(( mpl::or_< true_,unknown > ));
    MPL_ASSERT(( mpl::or_< true_,unknown,unknown > ));

    MPL_ASSERT_NOT(( mpl::not_< true_ > ));
    MPL_ASSERT(( mpl::not_< false_ > ));
}
