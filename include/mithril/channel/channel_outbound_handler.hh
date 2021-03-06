//
// Created by ssheikh on 2021-02-25.
//

#ifndef MITHRIL_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH
#define MITHRIL_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH

#include <mithril/channel/channel_handler.hh>
#include <mithril/message.hh>

class channel_outbound_handler
    : public virtual channel_handler
{
public:
  virtual void write(channel_handler_context& ctx, mithril::message msg) = 0;

  channel_direction direction() override
  {
    return channel_direction::outbound;
  }
};

class channel_outbound_handler_adapter
    : public channel_outbound_handler, public channel_handler_adapter
{
public:
  void write(channel_handler_context& ctx, mithril::message msg) override
  {
  }
};

#endif //MITHRIL_INCLUDE_CHANNEL_OUTBOUND_HANDLER_HH
