
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/min_max.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/min_max.hpp>
#include <boost/mpl/int.hpp>

#include <boost/mpl/aux_/test.hpp>


MPL_TEST_CASE()
{
    MPL_ASSERT(( is_same< mpl::min< int_<5>,int_<7> >::type,int_<5> > ));
    MPL_ASSERT(( is_same< mpl::max< int_<5>,int_<7> >::type,int_<7> > ));

    MPL_ASSERT(( is_same< mpl::min< int_<-5>,int_<-7> >::type,int_<-7> > ));
    MPL_ASSERT(( is_same< mpl::max< int_<-5>,int_<-7> >::type,int_<-5> > ));
}
