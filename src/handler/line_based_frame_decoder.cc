//
// Created by ssheikh on 2021-03-05.
//

#include <rune/channel/channel_handler_context.hh>
#include <rune/buffer/composite_buffer.hh>
#include <rune/handler/line_based_frame_decoder.hh>

#include <list>

void line_based_frame_decoder::decode(channel_handler_context&, composite_buffer& msg, std::list<rune::message>& out)
{
  if (msg.readable_bytes()) {
    std::basic_string<char> line;
    for (auto it = msg.ro_begin(); it != msg.ro_end(); ++it) {
      if (*it == '\n') [[unlikely]] {
        out.emplace_back(rune::create_message(byte_buffer::copy_of(line)));
        line = {};
      } else {
        if (line.size() < max_line_length) {
          line += *it;
        } else {
          line = {};
          while (*it != '\n' && it != msg.ro_end())
            ++it;
        }
      }
    }
  }
}