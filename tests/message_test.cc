//
// Created by ssheikh on 2021-03-01.
//

#include <rune/message.hh>

#include <gtest/gtest.h>

using rune::message;
using rune::create_message;

TEST(MessageTest, NoThrowHoldArbitraryTypes)
{
  ASSERT_NO_THROW(create_message(42));
  ASSERT_NO_THROW(create_message(42l));
  ASSERT_NO_THROW(create_message('a'));
}

TEST(MessageTest, HoldString)
{
  message msg = create_message(std::string("42"));
  ASSERT_TRUE(msg->holds<std::string>());
}

TEST(MessageTest, UnwrapSimpleTypes)
{
  message msg1 = create_message(42);
  ASSERT_EQ(msg1->unwrap<int>(), 42);

  message msg2 = create_message(42u);
  ASSERT_EQ(msg2->unwrap<unsigned int>(), 42);

  message msg3 = create_message('a');
  ASSERT_EQ(msg3->unwrap<char>(), 'a');

  message msg4 = create_message(std::string("42"));
  ASSERT_STREQ(msg4->unwrap<std::string>().c_str(), std::string("42").c_str());
}

TEST(MessageTest, ExpectBadCastException)
{
  message msg1 = create_message(42ul);
  ASSERT_TRUE(msg1->holds<unsigned long>());
  EXPECT_THROW((void)msg1->unwrap<char>(), rune::bad_message_cast_exception);
}

TEST(MessageTest, ExpectDoubleUnwrapException)
{
  message msg1 = create_message(42u);
  (void)msg1->unwrap<unsigned>();
  EXPECT_THROW((void)msg1->unwrap<unsigned>(), rune::double_message_unwrap_exception);
}

TEST(MessageTest, LValue)
{
  auto test_str = std::string("test");
  message msg = create_message(std::move(test_str));
  ASSERT_TRUE(msg->holds<std::string>());
  ASSERT_STREQ(msg->unwrap<std::string>().c_str(), std::string("test").c_str());
}

TEST(MessageTest, NonCopyableTest)
{
  struct noncopyable
  {
    noncopyable(const noncopyable&) = delete;

    noncopyable& operator=(const noncopyable&) = delete;

    noncopyable& operator=(noncopyable&&) = default;

    noncopyable(noncopyable&&) = default;

    noncopyable() = default;
  };

  noncopyable nc;
  message msg = create_message(std::move(nc));

  ASSERT_NO_THROW([&] {
    auto retrieved = msg->unwrap<noncopyable>();
  });
}

TEST(MessageTest, Destructor)
{
  {
    message msg = create_message(std::string(""));
  }
}