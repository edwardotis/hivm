
#ifndef BOOST_MPL_LIST_AUX_FRONT_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_FRONT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/boost/include/boost-1_33_1/boost/mpl/vector/aux_/front.hpp,v $
// $Date: 2006/08/02 04:29:37 $
// $Revision: 1.1 $

#include <boost/mpl/front_fwd.hpp>
#include <boost/mpl/vector/aux_/at.hpp>
#include <boost/mpl/vector/aux_/tag.hpp>
#include <boost/mpl/aux_/nttp_decl.hpp>
#include <boost/mpl/aux_/config/typeof.hpp>
#include <boost/mpl/aux_/config/ctps.hpp>

namespace boost { namespace mpl {

#if defined(BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct front_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
        : v_at<Vector,0>
    {
    };
};

#else

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< BOOST_MPL_AUX_NTTP_DECL(long, n_) >
struct front_impl< aux::vector_tag<n_> >
{
    template< typename Vector > struct apply
    {
        typedef typename Vector::item0 type;
    };
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // BOOST_MPL_LIST_AUX_FRONT_HPP_INCLUDED
