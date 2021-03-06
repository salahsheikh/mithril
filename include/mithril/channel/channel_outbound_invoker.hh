//
// Created by ssheikh on 2021-02-25.
//

#ifndef MITHRIL_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH
#define MITHRIL_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH

#include <mithril/channel/channel_inbound_invoker.hh>
#include <mithril/message.hh>

#include <seastar/core/future.hh>

class channel_outbound_invoker
{
  virtual channel_outbound_invoker& read() = 0;

  virtual void write(mithril::message msg) = 0;
};

#endif //MITHRIL_INCLUDE_CHANNEL_OUTBOUND_INVOKER_HH
