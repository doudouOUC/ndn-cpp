// Boost.Signals library

// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/signals

#ifndef NDNBOOST_VISIT_EACH_HPP
#define NDNBOOST_VISIT_EACH_HPP

#include <ndnboost/config.hpp>

namespace ndnboost {
  template<typename Visitor, typename T>
  inline void visit_each(Visitor& visitor, const T& t, long)
  {
    visitor(t);
  }

  template<typename Visitor, typename T>
  inline void visit_each(Visitor& visitor, const T& t)
  {
    visit_each(visitor, t, 0);
  }
}

#endif // NDNBOOST_VISIT_EACH_HPP
