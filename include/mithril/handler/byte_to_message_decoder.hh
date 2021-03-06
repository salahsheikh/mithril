//
// Created by ssheikh on 2021-03-03.
//

#ifndef RUNE_INCLUDE_RUNE_HANDLER_BYTE_TO_MESSAGE_DECODER_HH
#define RUNE_INCLUDE_RUNE_HANDLER_BYTE_TO_MESSAGE_DECODER_HH

#include <mithril/channel/channel_inbound_handler.hh>
#include <mithril/message.hh>
#include <mithril/buffer/byte_buffer.hh>
#include <mithril/buffer/composite_buffer.hh>

#include <list>

class byte_to_message_decoder
    : public channel_inbound_handler_adapter
{
private:

  // Merge buffers while minimizing memory copy
  class buffer_compositor
  {
    composite_buffer comp_buf;

    void expand(byte_buffer&& buf)
    {
      comp_buf.expand(std::move(buf));
    }

    friend byte_to_message_decoder;
  };

  std::list<mithril::message> decoded_messages;

  buffer_compositor ax;

public:

  virtual void decode(channel_handler_context&, composite_buffer& msg, std::list<mithril::message>& out) = 0;

  void channel_read(channel_handler_context& ctx, mithril::message msg) override;
};

#endif //RUNE_INCLUDE_RUNE_HANDLER_BYTE_TO_MESSAGE_DECODER_HH
