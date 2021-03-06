//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_INBOUND_HANDLER_HH
#define RUNE_INCLUDE_CHANNEL_INBOUND_HANDLER_HH

#include <mithril/channel/channel_handler.hh>
#include <mithril/message.hh>

class channel_inbound_handler
    : public virtual channel_handler
{
public:
  virtual void channel_active(channel_handler_context&) = 0;

  virtual void channel_inactive(channel_handler_context&) = 0;

  virtual void channel_read(channel_handler_context&, mithril::message) = 0;

  channel_direction direction() override
  {
    return channel_direction::inbound;
  }
};

class channel_inbound_handler_adapter
    : public virtual channel_inbound_handler, public channel_handler_adapter
{
public:
  void channel_active(channel_handler_context& ctx) override;

  void channel_inactive(channel_handler_context& ctx) override;

  void channel_read(channel_handler_context&, mithril::message) override;
};

#endif //RUNE_INCLUDE_CHANNEL_INBOUND_HANDLER_HH
