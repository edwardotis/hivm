
#ifndef BOOST_MPL_MULTISET_AUX_INSERT_IMPL_HPP_INCLUDED
#define BOOST_MPL_MULTISET_AUX_INSERT_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/multiset/aux_/insert_impl.hpp,v $
// $Date: 2006/08/07 05:19:40 $
// $Revision: 1.1 $

#include <boost/mpl/multiset/aux_/item.hpp>
#include <boost/mpl/multiset/aux_/tag.hpp>
#include <boost/mpl/insert_fwd.hpp>

namespace boost { namespace mpl {

template<>
struct insert_impl< aux::multiset_tag >
{
    template< typename Set, typename Key, typename unused_ > struct apply
    {
        typedef ms_item<Key,Set> type;
    };
};

}}

#endif // BOOST_MPL_MULTISET_AUX_INSERT_IMPL_HPP_INCLUDED
