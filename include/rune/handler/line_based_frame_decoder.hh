//
// Created by ssheikh on 2021-03-03.
//

#ifndef RUNE_INCLUDE_RUNE_HANDLER_LINE_BASED_FRAME_DECODER_HH
#define RUNE_INCLUDE_RUNE_HANDLER_LINE_BASED_FRAME_DECODER_HH

#include <rune/channel/channel_inbound_handler.hh>
#include <rune/handler/byte_to_message_decoder.hh>
#include <rune/message.hh>

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

  void decode(channel_handler_context&, composite_buffer& msg, std::list<rune::message>& out) override;
};

#endif //RUNE_INCLUDE_RUNE_HANDLER_LINE_BASED_FRAME_DECODER_HH
