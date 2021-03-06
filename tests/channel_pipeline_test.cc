//
// Created by ssheikh on 2021-02-20.
//

#include "test_channel.h"

#include <rune/channel/channel_pipeline.hh>
#include <rune/channel/channel_initializer.hh>
#include <rune/message.hh>

#include <gtest/gtest.h>

class string_decode_channel_initializer
    : public channel_initializer
{
public:
  void init_channel(channel_handler_context& ctx) override
  {

  }
};

TEST(ChannelPipelineTest, FireEventsOnEmptyPipeline)
{
  auto channel = std::make_shared<test_channel>();
  auto pipeline = std::make_shared<channel_pipeline>();
  channel->attach_pipeline(pipeline);
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_active());
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_read(rune::create_message(std::string())));
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_inactive());
}

TEST(ChannelPipelineTest, FireEventsOnInitializedPipeline)
{
  auto channel = std::make_shared<test_channel>();
  auto pipeline = std::make_shared<channel_pipeline>();
  channel->attach_pipeline(pipeline);
  channel->pipeline()->add_last<string_decode_channel_initializer>("channel_initializer");
  channel->pipeline()->remove("channel_initializer");
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_active());
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_read(rune::create_message(std::string())));
  ASSERT_NO_THROW(channel->pipeline()->fire_channel_inactive());
}
