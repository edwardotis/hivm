
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License,Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/libs/mpl/test/integral_c.cpp,v $
// $Date: 2006/08/07 05:35:05 $
// $Revision: 1.1 $

#include <boost/mpl/integral_c.hpp>
#include <boost/preprocessor/repeat.hpp>

#include "integral_wrapper_test.hpp"


MPL_TEST_CASE()
{
#   define WRAPPER(T, i) integral_c<T,i>

#if !(defined(__APPLE_CC__) && defined(__GNUC__) && (__GNUC__ == 3) && (__GNUC_MINOR__ <= 3))
    BOOST_PP_REPEAT(10, INTEGRAL_WRAPPER_TEST, char)
#endif
    BOOST_PP_REPEAT(10, INTEGRAL_WRAPPER_TEST, short)
    BOOST_PP_REPEAT(10, INTEGRAL_WRAPPER_TEST, int)
    BOOST_PP_REPEAT(10, INTEGRAL_WRAPPER_TEST, long)
}
