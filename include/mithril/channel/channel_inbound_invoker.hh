//
// Created by ssheikh on 2021-02-25.
//

#ifndef MITHRIL_INCLUDE_CHANNEL_INBOUND_INVOKER_HH
#define MITHRIL_INCLUDE_CHANNEL_INBOUND_INVOKER_HH

#include <mithril/message.hh>

class channel_inbound_invoker
{
public:
  virtual void fire_channel_active() = 0;

  virtual void fire_channel_inactive() = 0;

  virtual void fire_channel_read(mithril::message) = 0;
};

#endif //MITHRIL_INCLUDE_CHANNEL_INBOUND_INVOKER_HH
