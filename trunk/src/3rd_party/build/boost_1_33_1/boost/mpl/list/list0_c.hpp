
#ifndef BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED
#define BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/list/list0_c.hpp,v $
// $Date: 2006/08/07 05:19:40 $
// $Revision: 1.1 $

#include <boost/mpl/list/list0.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost { namespace mpl {

template< typename T > struct list0_c
    : l_end
{
    typedef l_end type;
    typedef T value_type;
};

}}

#endif // BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED
