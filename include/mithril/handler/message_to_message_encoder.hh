//
// Created by ssheikh on 2021-03-04.
//

#ifndef RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_ENCODER_HH
#define RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_ENCODER_HH

#include <mithril/channel/channel_outbound_handler.hh>
#include <mithril/message.hh>
#include <mithril/composite_buffer.hh>

#include <list>

template<typename Input>
class message_to_message_encoder
    : public channel_outbound_handler_adapter
{
private:
  std::list<mithril::message> out;

public:
  virtual void encode(channel_handler_context&, Input msg, std::list<mithril::message>& out) = 0;

  void write(channel_handler_context& ctx, mithril::message msg) override
  {
    if (msg->template holds<Input>()) {
      encode(ctx, msg->unwrap<Input>(), out);

      for (auto&& out_msg : out) {
        ctx.write(std::move(out_msg));
      }
      out.clear();
    } else {
      ctx.write(std::move(msg));
    }
  }
};

#endif //RUNE_INCLUDE_RUNE_HANDLER_MESSAGE_TO_MESSAGE_ENCODER_HH
