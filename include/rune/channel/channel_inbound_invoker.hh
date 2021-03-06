//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_INBOUND_INVOKER_HH
#define RUNE_INCLUDE_CHANNEL_INBOUND_INVOKER_HH

#include <rune/message.hh>

class channel_inbound_invoker
{
public:
  virtual void fire_channel_active() = 0;

  virtual void fire_channel_inactive() = 0;

  virtual void fire_channel_read(rune::message) = 0;
};

#endif //RUNE_INCLUDE_CHANNEL_INBOUND_INVOKER_HH
