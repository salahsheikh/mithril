//
// Created by ssheikh on 2021-03-03.
//

#ifndef RUNE_INCLUDE_RUNE_HANDLER_STRING_DECODER_HH
#define RUNE_INCLUDE_RUNE_HANDLER_STRING_DECODER_HH

#include <mithril/handler/message_to_message_decoder.hh>
#include <mithril/buffer/byte_buffer.hh>

#include <string>

class string_decoder
    : public message_to_message_decoder<byte_buffer>
{
public:
  void decode(channel_handler_context&, byte_buffer msg, std::list<mithril::message>& out) override
  {
    out.push_back(mithril::create_message(to_string(msg)));
  }
};

#endif //RUNE_INCLUDE_RUNE_HANDLER_STRING_DECODER_HH
