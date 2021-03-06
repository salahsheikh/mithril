//
// Created by ssheikh on 2021-03-03.
//

#include <rune/buffer/byte_buffer.hh>
#include <rune/buffer/composite_buffer.hh>

#include <gtest/gtest.h>

TEST(BufferTest, Expand)
{
  composite_buffer comp_buf;

  std::string s1("cat");
  auto tbuf1 = byte_buffer::copy_of(s1);
  comp_buf.expand(std::move(tbuf1));
  ASSERT_EQ(comp_buf.readable_bytes(), s1.size());

  std::string s2("dog");
  auto tbuf2 = byte_buffer::copy_of(s2);
  comp_buf.expand(std::move(tbuf2));
  ASSERT_EQ(comp_buf.readable_bytes(), s1.size() + s2.size());
}

TEST(BufferTest, CrossBoundaryRead)
{
  composite_buffer comp_buf;

  std::string s1("aaa");
  byte_buffer tbuf1 = byte_buffer::copy_of(s1);
  comp_buf.expand(std::move(tbuf1));

  std::string s2("bb");
  byte_buffer tbuf2 = byte_buffer::copy_of(s2);
  comp_buf.expand(std::move(tbuf2));

  std::string s3("c");
  byte_buffer tbuf3 = byte_buffer::copy_of(s3);
  comp_buf.expand(std::move(tbuf3));

  auto it = comp_buf.ro_begin();
  ASSERT_EQ(*it, 'a');
  ++it;
  ASSERT_EQ(*it, 'a');
  ++it;
  ASSERT_EQ(*it, 'a');
  ++it;
  ASSERT_EQ(*it, 'b');
  ++it;
  ASSERT_EQ(*it, 'b');
  ++it;
  ASSERT_EQ(*it, 'c');
  ++it;
}

TEST(BufferTest, AccumulateComposite)
{
  composite_buffer comp_buf;

  std::string s1("aaa");
  byte_buffer tbuf1 = byte_buffer::copy_of(s1);
  comp_buf.expand(std::move(tbuf1));

  std::string s2("bb");
  byte_buffer tbuf2 = byte_buffer::copy_of(s2);
  comp_buf.expand(std::move(tbuf2));

  std::string s3("c");
  byte_buffer tbuf3 = byte_buffer::copy_of(s3);
  comp_buf.expand(std::move(tbuf3));

  std::string composed;
  for (auto it = comp_buf.ro_begin(); it != comp_buf.ro_end(); ++it) {
    composed += *it;
  }

  ASSERT_STREQ(composed.c_str(), "aaabbc");
}