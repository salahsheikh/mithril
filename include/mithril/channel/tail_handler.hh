//
// Created by ssheikh on 2021-02-27.
//

#ifndef MITHRIL_INCLUDE_TAIL_HANDLER_HH
#define MITHRIL_INCLUDE_TAIL_HANDLER_HH

#include <mithril/channel/channel_handler.hh>
#include <mithril/channel/channel_inbound_handler.hh>
#include <mithril/channel/channel_outbound_handler.hh>
#include <mithril/channel/channel_handler_context.hh>
#include <mithril/message.hh>

#include <memory>

class channel;

class tail_handler final
    : public channel_inbound_handler_adapter
{
public:
  void channel_active(channel_handler_context& ctx) final;

  void channel_inactive(channel_handler_context& ctx) final;

  void channel_read(channel_handler_context&, mithril::message) final;
};

#endif //MITHRIL_INCLUDE_TAIL_HANDLER_HH
