
#ifndef BOOST_MPL_AUX_APPLY_1ST_HPP_INCLUDED
#define BOOST_MPL_AUX_APPLY_1ST_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/aux_/apply_1st.hpp,v $
// $Date: 2006/08/07 05:18:23 $
// $Revision: 1.1 $

#include <boost/mpl/apply.hpp>

namespace boost { namespace mpl { namespace aux {

struct apply_1st
{
    template< typename Pair, typename T > struct apply
        : apply2<
              typename Pair::first
            , typename Pair::second
            , T
            >
    {
    };
};

}}}

#endif // BOOST_MPL_AUX_APPLY_1ST_HPP_INCLUDED