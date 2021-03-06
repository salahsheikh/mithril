//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH
#define RUNE_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH

#include <rune/channel/channel_handler.hh>
#include <rune/message.hh>

class channel_outbound_handler
    : public virtual channel_handler
{
public:
  virtual void write(channel_handler_context& ctx, rune::message msg) = 0;

  channel_direction direction() override
  {
    return channel_direction::outbound;
  }
};

class channel_outbound_handler_adapter
    : public channel_outbound_handler, public channel_handler_adapter
{
public:
  void write(channel_handler_context& ctx, rune::message msg) override
  {
  }
};

#endif //RUNE_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH
