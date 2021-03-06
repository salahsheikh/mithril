//
// Created by ssheikh on 2021-03-04.
//

#ifndef MITHRIL_INCLUDE_MITHRIL_BUFFER_BYTE_BUFFER_HH
#define MITHRIL_INCLUDE_MITHRIL_BUFFER_BYTE_BUFFER_HH

#include <cstring>
#include <seastar/core/temporary_buffer.hh>

/// TODO: replace this with a buffer with an iterator and can discard previously read bytes
using byte_buffer = seastar::temporary_buffer<char>;

static std::string to_string(byte_buffer& buf)
{
  return std::string(buf.get(), buf.size());
}

#endif //MITHRIL_INCLUDE_MITHRIL_BUFFER_BYTE_BUFFER_HH
