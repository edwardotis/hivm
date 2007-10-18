
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/erase_range.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/erase.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef list<int,char,long,short,unsigned,long,double,long> types;

    typedef find<types,short>::type iter1;
    typedef find<types,double>::type iter2;

    typedef erase<types,iter1,iter2>::type result;
    
    MPL_ASSERT_RELATION( size<result>::value, ==, 5 );

    typedef find<result,unsigned>::type iter;
    MPL_ASSERT(( is_same< iter, end<result>::type > ));
}
