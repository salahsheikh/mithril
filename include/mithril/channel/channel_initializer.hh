//
// Created by ssheikh on 2021-02-26.
//

#ifndef MITHRIL_INCLUDE_CHANNEL_INITIALIZER_HH
#define MITHRIL_INCLUDE_CHANNEL_INITIALIZER_HH

#include <mithril/channel/channel_handler.hh>
#include <mithril/channel/channel_inbound_handler.hh>

class channel_initializer
    : public channel_inbound_handler_adapter
{
public:
  virtual void init_channel(channel_handler_context&) = 0;

  void handler_added(channel_handler_context& ctx) override
  {
    init_channel(ctx);
  }
};

#endif //MITHRIL_INCLUDE_CHANNEL_INITIALIZER_HH
