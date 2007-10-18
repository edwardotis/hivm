
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/unpack_args.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/unpack_args.hpp>

#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/aux_/test.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( apply1< 
          unpack_args< is_same<_1,_2> >
        , vector2<int,int>
        > ));
}
