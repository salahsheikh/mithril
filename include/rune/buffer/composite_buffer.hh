//
// Created by ssheikh on 2021-03-03.
//

#ifndef RUNE_INCLUDE_RUNE_COMPOSITE_BUFFER_HH
#define RUNE_INCLUDE_RUNE_COMPOSITE_BUFFER_HH

#include <rune/common.hh>
#include <rune/buffer/byte_buffer.hh>

#include <vector>
#include <utility>

/// Present a contiguous view of one or more buffers
class composite_buffer
{
private:
  std::vector<byte_buffer> m_buffers;
  size_t m_readable_bytes;

  struct cursor {
    size_t slice_index {0}, read_index {0};
  };

  class iterator
  {
  private:
    composite_buffer& cbuf;
    composite_buffer::cursor cursor;
  public:
    explicit iterator(composite_buffer& cbuf)
        : cbuf {cbuf}
    {
      cursor.slice_index = cbuf.m_slice_index;
      cursor.read_index = cbuf.m_read_index;
    }

    explicit iterator(composite_buffer& cbuf, size_t spos, size_t rpos)
        : cbuf {cbuf}
    {
      cursor.slice_index = spos;
      cursor.read_index = rpos;
    }

    iterator& operator++()
    {
      auto current_boundary = cbuf.m_buffers[cursor.slice_index].size();
      if (cursor.read_index < current_boundary) {
        cursor.read_index++;
      }

      if (cursor.read_index == current_boundary) {
        cursor.read_index = 0;
        cursor.slice_index++;
      }

      return *this;
    }

    bool operator!=(const iterator& other)
    {
      return std::tie(cursor.read_index, cursor.slice_index) != std::tie(other.cursor.read_index, other.cursor.slice_index);
    }

    char operator*()
    {
      return cbuf.m_buffers[cursor.slice_index][cursor.read_index];
    }
  };

  composite_buffer::cursor cursor;

  size_t m_slice_index;
  size_t m_read_index;
public:

  composite_buffer()
      : m_slice_index {0}, m_read_index {0}, m_readable_bytes {0}
  {
  }

  void expand(byte_buffer&& buf)
  {
    m_readable_bytes += buf.size();
    m_buffers.emplace_back(std::move(buf));
  }

  explicit operator bool() const noexcept
  {
    return m_readable_bytes >= 0;
  }

  void advance()
  {
    auto current_boundary = m_buffers[m_slice_index].size();
    if (m_read_index < current_boundary) {
      m_read_index++;
    }

    if (m_read_index == current_boundary) {
      m_read_index = 0;
      m_slice_index++;
    }
  }

  char operator*()
  {
    return m_buffers[m_slice_index][m_read_index];
  }

  composite_buffer::iterator ro_begin()
  {
    return composite_buffer::iterator(*this);
  }

  composite_buffer::iterator ro_end()
  {
    return composite_buffer::iterator(*this, m_buffers.size(), 0);
  }

  [[nodiscard]] size_t readable_bytes() const
  {
    return m_readable_bytes;
  }
};

#endif //RUNE_INCLUDE_RUNE_COMPOSITE_BUFFER_HH
