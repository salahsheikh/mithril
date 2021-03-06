//
// Created by ssheikh on 2021-02-26.
//

#ifndef RUNE_TESTS_TEST_CHANNEL_H
#define RUNE_TESTS_TEST_CHANNEL_H

#include <rune/channel/channel.hh>

class test_channel
    : public channel
{
public:

  std::list<rune::message> tx;

  test_channel& read() override
  {
    return *this;
  }

  void write(rune::message msg) override
  {
    tx.push_back(std::move(msg));
  }
};

#endif //RUNE_TESTS_TEST_CHANNEL_H
