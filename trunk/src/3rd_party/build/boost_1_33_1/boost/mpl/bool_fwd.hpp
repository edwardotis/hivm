
#ifndef BOOST_MPL_BOOL_FWD_HPP_INCLUDED
#define BOOST_MPL_BOOL_FWD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/bool_fwd.hpp,v $
// $Date: 2006/08/07 05:19:38 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/adl_barrier.hpp>

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< bool C_ > struct bool_;

// shorcuts
typedef bool_<true> true_;
typedef bool_<false> false_;

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

BOOST_MPL_AUX_ADL_BARRIER_DECL(bool_)
BOOST_MPL_AUX_ADL_BARRIER_DECL(true_)
BOOST_MPL_AUX_ADL_BARRIER_DECL(false_)

#endif // BOOST_MPL_BOOL_FWD_HPP_INCLUDED
