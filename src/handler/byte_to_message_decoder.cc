//
// Created by ssheikh on 2021-03-05.
//

#include <mithril/handler/byte_to_message_decoder.hh>
#include <mithril/channel/channel_handler_context.hh>

void byte_to_message_decoder::channel_read(channel_handler_context& ctx, mithril::message msg)
{
  if (msg->holds<byte_buffer>()) {
    ax.expand(std::move(msg->unwrap<byte_buffer>()));
    decode(ctx, ax.comp_buf, decoded_messages);
    while (!decoded_messages.empty()) {
      ctx.fire_channel_read(std::move(decoded_messages.front()));
      decoded_messages.pop_front();
    }
  } else {
    ctx.fire_channel_read(std::move(msg));
  }
}
