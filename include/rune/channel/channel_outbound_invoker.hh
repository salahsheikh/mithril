//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH
#define RUNE_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH

#include <rune/channel/channel_inbound_invoker.hh>
#include <rune/message.hh>

#include <seastar/core/future.hh>

class channel_outbound_invoker
{
  virtual channel_outbound_invoker& read() = 0;

  virtual void write(rune::message msg) = 0;
};

#endif //RUNE_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH
