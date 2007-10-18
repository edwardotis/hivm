
#ifndef BOOST_MPL_AUX_RANGE_C_EMPTY_HPP_INCLUDED
#define BOOST_MPL_AUX_RANGE_C_EMPTY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/aux_/range_c/empty.hpp,v $
// $Date: 2006/08/07 05:19:38 $
// $Revision: 1.1 $

#include <boost/mpl/empty_fwd.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/aux_/range_c/tag.hpp>

namespace boost { namespace mpl {

template<>
struct empty_impl< aux::half_open_range_tag >
{
    template< typename Range > struct apply
        : equal_to<
              typename Range::start
            , typename Range::finish
            >
    {
    };
};

}}

#endif // BOOST_MPL_AUX_RANGE_C_EMPTY_HPP_INCLUDED
