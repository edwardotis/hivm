
#ifndef BOOST_MPL_SET_SET0_C_HPP_INCLUDED
#define BOOST_MPL_SET_SET0_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/boost/include/boost-1_33_1/boost/mpl/set/set0_c.hpp,v $
// $Date: 2006/08/02 04:29:37 $
// $Revision: 1.1 $

#include <boost/mpl/set/set0.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost { namespace mpl {

template< typename T > struct set0_c
    : set0<>
{
    typedef set0_c type;
    typedef T value_type;
};

}}

#endif // BOOST_MPL_SET_SET0_C_HPP_INCLUDED