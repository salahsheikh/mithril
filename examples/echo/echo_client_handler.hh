//
// Created by ssheikh on 2021-03-04.
//

#ifndef RUNE_EXAMPLES_ECHO_ECHO_CLIENT_HANDLER_HH
#define RUNE_EXAMPLES_ECHO_ECHO_CLIENT_HANDLER_HH

#include <rune/channel/channel_inbound_handler.hh>
#include <rune/channel/channel_handler_context.hh>
#include <rune/handler/byte_to_message_decoder.hh>
#include <rune/channel/channel_handler_context.hh>
#include <rune/handler/line_based_frame_decoder.hh>
#include <rune/handler/string_decoder.hh>

#include <iostream>
#include <exception>

class echo_server_handler
    : public channel_inbound_handler_adapter
{
public:
  void channel_active(channel_handler_context& ctx) override
  {
    std::cout << "Accepted connection!" << std::endl;
    ctx.write(rune::create_message(byte_buffer::copy_of("test test")));
  }

  void channel_inactive(channel_handler_context&) override
  {
    std::cout << "Disconnected connection" << std::endl;
  }

  void channel_read(channel_handler_context& ctx, rune::message msg) override
  {
    if (msg->holds<byte_buffer>()) {
      auto buf = msg->unwrap<byte_buffer>();
      std::cout << "Read: " << to_string(buf) << std::endl;
      ctx.write(std::move(buf));
    }
  }
};

#endif //RUNE_EXAMPLES_ECHO_ECHO_CLIENT_HANDLER_HH
