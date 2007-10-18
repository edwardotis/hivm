
#ifndef BOOST_MPL_AUX_FOLD_OP_HPP_INCLUDED
#define BOOST_MPL_AUX_FOLD_OP_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/boost/include/boost-1_33_1/boost/mpl/aux_/fold_op.hpp,v $
// $Date: 2006/08/02 04:27:43 $
// $Revision: 1.1 $

#include <boost/mpl/apply.hpp>

namespace boost { namespace mpl { namespace aux {

// hand-written version is more efficient than bind/lambda expression
template< typename Op >
struct fold_op
{
    template< typename T1, typename T2 > struct apply
    {
        typedef typename apply2<
              Op
            , T1
            , typename T2::type
            >::type type;
    };
};

}}}

#endif // BOOST_MPL_AUX_FOLD_OP_HPP_INCLUDED
