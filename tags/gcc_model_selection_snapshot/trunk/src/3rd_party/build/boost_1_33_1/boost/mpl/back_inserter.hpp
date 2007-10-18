
#ifndef BOOST_MPL_BACK_INSERTER_HPP_INCLUDED
#define BOOST_MPL_BACK_INSERTER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/build/boost_1_33_1/boost/mpl/back_inserter.hpp,v $
// $Date: 2006/08/07 05:19:38 $
// $Revision: 1.1 $

#include <boost/mpl/push_back.hpp>
#include <boost/mpl/inserter.hpp>

namespace boost {
namespace mpl {

template<
      typename Sequence
    >
struct back_inserter
    : inserter< Sequence,push_back<> >
{
};

}}

#endif // BOOST_MPL_BACK_INSERTER_HPP_INCLUDED
