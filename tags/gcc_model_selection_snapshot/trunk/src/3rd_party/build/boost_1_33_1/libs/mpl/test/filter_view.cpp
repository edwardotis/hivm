
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/filter_view.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/static_assert.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/aux_/test.hpp>

#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    typedef mpl::list<int,float,long,float,char[50],long double,char> types;
    typedef mpl::max_element<
          mpl::transform_view<
              mpl::filter_view< types,boost::is_float<_> >
            , mpl::sizeof_<_>
            >
        >::type iter;

    MPL_ASSERT((is_same<mpl::deref<iter::base>::type, long double>));
}
