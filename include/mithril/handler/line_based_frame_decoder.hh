//
// Created by ssheikh on 2021-03-03.
//

#ifndef MITHRIL_INCLUDE_MITHRIL_HANDLER_LINE_BASED_FRAME_DECODER_HH
#define MITHRIL_INCLUDE_MITHRIL_HANDLER_LINE_BASED_FRAME_DECODER_HH

#include <mithril/channel/channel_inbound_handler.hh>
#include <mithril/handler/byte_to_message_decoder.hh>
#include <mithril/message.hh>

class line_based_frame_decoder
    : public byte_to_message_decoder
{
private:
  const size_t max_line_length;
public:
  explicit line_based_frame_decoder(size_t max_line_length = 8192)
      : max_line_length {max_line_length}
  {
  }

  void decode(channel_handler_context&, composite_buffer& msg, std::list<mithril::message>& out) override;
};

#endif //MITHRIL_INCLUDE_MITHRIL_HANDLER_LINE_BASED_FRAME_DECODER_HH
