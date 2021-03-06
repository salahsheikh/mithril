//
// Created by ssheikh on 2021-03-04.
//

#ifndef RUNE_INCLUDE_RUNE_BUFFER_BYTE_BUFFER_HH
#define RUNE_INCLUDE_RUNE_BUFFER_BYTE_BUFFER_HH

#include <cstring>
#include <seastar/core/temporary_buffer.hh>

/// TODO: replace this with a buffer with an iterator and can discard previously read bytes
using byte_buffer = seastar::temporary_buffer<char>;

static std::string to_string(byte_buffer& buf)
{
  return std::string(buf.get(), buf.size());
}

#endif //RUNE_INCLUDE_RUNE_BUFFER_BYTE_BUFFER_HH
