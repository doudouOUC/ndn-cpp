
#ifndef NDNBOOST_MPL_VECTOR_AUX_CLEAR_HPP_INCLUDED
#define NDNBOOST_MPL_VECTOR_AUX_CLEAR_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: clear.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <ndnboost/mpl/clear_fwd.hpp>
#include <ndnboost/mpl/vector/aux_/vector0.hpp>
#include <ndnboost/mpl/vector/aux_/tag.hpp>
#include <ndnboost/mpl/aux_/config/typeof.hpp>
#include <ndnboost/mpl/aux_/config/ctps.hpp>

namespace ndnboost { namespace mpl {

#if defined(NDNBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct clear_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
    {
        typedef vector0<> type;
    };
};

#else

#if !defined(NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct clear_impl< aux::vector_tag<N> >
{
    template< typename Vector > struct apply
    {
        typedef vector0<> type;
    };
};

#endif // NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // NDNBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // NDNBOOST_MPL_VECTOR_AUX_CLEAR_HPP_INCLUDED
