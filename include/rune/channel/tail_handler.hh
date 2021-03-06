//
// Created by ssheikh on 2021-02-27.
//

#ifndef RUNE_INCLUDE_TAIL_HANDLER_HH
#define RUNE_INCLUDE_TAIL_HANDLER_HH

#include <rune/channel/channel_handler.hh>
#include <rune/channel/channel_inbound_handler.hh>
#include <rune/channel/channel_outbound_handler.hh>
#include <rune/channel/channel_handler_context.hh>
#include <rune/message.hh>

#include <memory>

class channel;

class tail_handler final
    : public channel_inbound_handler_adapter
{
public:
  void channel_active(channel_handler_context& ctx) final;

  void channel_inactive(channel_handler_context& ctx) final;

  void channel_read(channel_handler_context&, rune::message) final;
};

#endif //RUNE_INCLUDE_TAIL_HANDLER_HH
