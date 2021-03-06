//
// Created by ssheikh on 2021-02-26.
//

#ifndef MITHRIL_TESTS_TEST_CHANNEL_H
#define MITHRIL_TESTS_TEST_CHANNEL_H

#include <mithril/channel/channel.hh>

class test_channel
    : public channel
{
public:

  std::list<mithril::message> tx;

  test_channel& read() override
  {
    return *this;
  }

  void write(mithril::message msg) override
  {
    tx.push_back(std::move(msg));
  }
};

#endif //MITHRIL_TESTS_TEST_CHANNEL_H
