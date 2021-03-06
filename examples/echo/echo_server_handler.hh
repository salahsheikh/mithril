//
// Created by ssheikh on 2021-03-04.
//

#ifndef MITHRIL_EXAMPLES_ECHO_ECHO_SERVER_HANDLER_HH
#define MITHRIL_EXAMPLES_ECHO_ECHO_SERVER_HANDLER_HH

#include <mithril/common.hh>
#include <mithril/channel/channel_handler_context.hh>
#include <mithril/buffer/byte_buffer.hh>

#include <iostream>
#include <exception>

class echo_server_handler
    : public channel_inbound_handler_adapter
{
public:
  void channel_active(channel_handler_context& ctx) override
  {
    std::cout << "Accepted connection!" << std::endl;
  }

  void channel_inactive(channel_handler_context&) override
  {
    std::cout << "Disconnected connection!" << std::endl;
  }

  void channel_read(channel_handler_context& ctx, mithril::message msg) override
  {
    if (msg->holds<byte_buffer>()) {
      auto buf = msg->unwrap<byte_buffer>();
      std::cout << "Read: " << to_string(buf) << std::endl;
      ctx.write(std::move(buf));
    }
  }
};

#endif //MITHRIL_EXAMPLES_ECHO_ECHO_SERVER_HANDLER_HH
