//
// Created by ssheikh on 2021-03-04.
//

#include "test_channel.hh"

#include <mithril/channel/channel_pipeline.hh>
#include <mithril/channel/channel_initializer.hh>
#include <mithril/handler/line_based_frame_decoder.hh>
#include <mithril/message.hh>
#include <mithril/buffer/byte_buffer.hh>

#include <gtest/gtest.h>

class read_then_write_handler
    : public channel_inbound_handler_adapter
{
public:
  void channel_read(channel_handler_context& ctx, mithril::message msg) override
  {
    ctx.write(std::move(msg));
  }
};

class string_decode_channel_initializer
    : public channel_initializer
{
private:
  size_t max_len;
public:
  string_decode_channel_initializer(size_t max_len)
      : max_len {max_len}
  {

  }

  void init_channel(channel_handler_context& ctx) override
  {
    ctx.pipeline().add_last<line_based_frame_decoder>("line_decoder", max_len);
    ctx.pipeline().add_last<read_then_write_handler>("rw_handler");
  }
};

TEST(LineBasedFrameDecoderTest, SimpleDecode)
{
  auto channel = std::make_shared<test_channel>();
  auto pipeline = std::make_shared<channel_pipeline>();
  channel->attach_pipeline(pipeline);
  channel->pipeline()->add_last<string_decode_channel_initializer>("channel_initializer", 8192);
  channel->pipeline()->remove("channel_initializer");

  auto test_string_buf = byte_buffer::copy_of("one\ntwo\nthree\n");
  ASSERT_TRUE(channel->tx.empty());

  channel->pipeline()->fire_channel_read(mithril::create_message(std::move(test_string_buf)));

  ASSERT_EQ(channel->tx.size(), 3);
}

TEST(LineBasedFrameDecoderTest, DecodeTooLong)
{
  auto channel = std::make_shared<test_channel>();
  auto pipeline = std::make_shared<channel_pipeline>();
  channel->attach_pipeline(pipeline);
  channel->pipeline()->add_last<string_decode_channel_initializer>("channel_initializer", 3);
  channel->pipeline()->remove("channel_initializer");

  auto test_string_buf = byte_buffer::copy_of("aaa\nbbbb\naaa\n");
  ASSERT_TRUE(channel->tx.empty());

  channel->pipeline()->fire_channel_read(mithril::create_message(std::move(test_string_buf)));

  // Discard long lines
  ASSERT_EQ(channel->tx.size(), 2);
}