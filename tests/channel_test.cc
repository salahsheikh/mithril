//
// Created by ssheikh on 2021-02-20.
//

#include "test_channel.h"

#include <gtest/gtest.h>

TEST(ChannelTest, Creation)
{
  ASSERT_NO_THROW(test_channel());
  ASSERT_FALSE(test_channel().pipeline());
}