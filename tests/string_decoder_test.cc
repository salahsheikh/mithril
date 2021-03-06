//
// Created by ssheikh on 2021-03-04.
//

#include "test_channel.h"

#include <rune/channel/channel_pipeline.hh>
#include <rune/channel/channel_initializer.hh>
#include <rune/handler/string_decoder.hh>
#include <rune/message.hh>
#include <rune/buffer/byte_buffer.hh>

#include <gtest/gtest.h>

class read_then_write_handler
    : public channel_inbound_handler_adapter
{
public:
  void channel_read(channel_handler_context& ctx, rune::message msg) override
  {
    ctx.write(std::move(msg));
  }
};

class string_decode_channel_initializer
    : public channel_initializer
{
public:
  void init_channel(channel_handler_context& ctx) override
  {
    ctx.pipeline().add_last<string_decoder>("str_decoder");
    ctx.pipeline().add_last<read_then_write_handler>("rw_handler");
  }
};

TEST(StringDecode, SimpleDecode)
{
  auto channel = std::make_shared<test_channel>();
  auto pipeline = std::make_shared<channel_pipeline>();
  channel->attach_pipeline(pipeline);
  channel->pipeline()->add_last<string_decode_channel_initializer>("channel_initializer");
  channel->pipeline()->remove("channel_initializer");

  std::string test_string("test");
  auto test_string_buf = byte_buffer::copy_of(test_string);
  channel->pipeline()->fire_channel_read(rune::create_message(std::move(test_string_buf)));
  ASSERT_TRUE(!channel->tx.empty());
  ASSERT_TRUE(channel->tx.front()->holds<std::string>());

  std::string recv_string = channel->tx.front()->unwrap<std::string>();
  ASSERT_STREQ(recv_string.c_str(), test_string.c_str());
}