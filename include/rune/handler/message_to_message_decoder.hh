//
// Created by ssheikh on 2021-03-03.
//

#ifndef RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_DECODER_HH
#define RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_DECODER_HH

#include <rune/channel/channel_inbound_handler.hh>
#include <rune/message.hh>
#include <rune/buffer/composite_buffer.hh>

#include <list>

template<typename Input>
class message_to_message_decoder
    : public channel_inbound_handler_adapter
{
private:
  std::list<rune::message> out;

public:

  virtual void decode(channel_handler_context&, Input msg, std::list<rune::message>& out) = 0;

  void channel_read(channel_handler_context& ctx, rune::message msg) override
  {
    if (msg->template holds<Input>()) {
      decode(ctx, msg->unwrap<Input>(), out);

      for (auto&& out_msg : out) {
        ctx.fire_channel_read(std::move(out_msg));
      }
      out.clear();
    } else {
      ctx.fire_channel_read(std::move(msg));
    }
  }
};

#endif //RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_DECODER_HH
