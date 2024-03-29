
#ifndef BOOST_MPL_AUX_MSVC_TYPE_HPP_INCLUDED
#define BOOST_MPL_AUX_MSVC_TYPE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/project/cvs/hivm/version2/trunk/src/3rd_party/boost/include/boost-1_33_1/boost/mpl/aux_/msvc_type.hpp,v $
// $Date: 2006/08/02 04:27:43 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/eti.hpp>
#include <boost/mpl/aux_/is_msvc_eti_arg.hpp>

namespace boost { namespace mpl { namespace aux {

#if defined(BOOST_MPL_CFG_MSVC_70_ETI_BUG)

template< bool > struct msvc_type_impl
{
    template< typename T > struct result_
    {
        typedef typename T::type type;
    };
};

template<> struct msvc_type_impl<true>
{
    template< typename T > struct result_
    {
        typedef result_ type;
    };
};

template< typename T > struct msvc_type
    : msvc_type_impl< is_msvc_eti_arg<T>::value >
        ::template result_<T>
{
};

#else // BOOST_MPL_CFG_MSVC_70_ETI_BUG

template< typename T > struct msvc_type 
{
    typedef typename T::type type;
};

template<> struct msvc_type<int>
{
    typedef int type;
};

#endif

}}}

#endif // BOOST_MPL_AUX_MSVC_TYPE_HPP_INCLUDED
