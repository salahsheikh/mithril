//
// Created by ssheikh on 2021-02-26.
//

#ifndef MITHRIL_INCLUDE_UTIL_H
#define MITHRIL_INCLUDE_UTIL_H

#ifndef CHECK
#include <cstdlib>

# define CHECK(expr) if (!expr) [[unlikely]] { std::abort(); }
#endif

#ifdef DCHECK_IS_ON
#  define DCHECK(...) do { CHECK(__VA_ARGS__) } while (false)
#else
#  define DCHECK(expr) ((void) expr)
#endif

#ifndef DEBUG_LOGGING
struct noop_log {
  template <typename T>
  noop_log& operator<<(const T&) {
  }
};
enum levels
{
  trace,
  debug,
  info,
  warning,
  error,
  fatal
};
static noop_log sink;
#define MITHRIL_LOG(lvl) (sink)
#else
#include <boost/log/trivial.hpp>

#define MITHRIL_LOG(lvl) BOOST_LOG_TRIVIAL(lvl)
#endif

#endif //MITHRIL_INCLUDE_UTIL_H
