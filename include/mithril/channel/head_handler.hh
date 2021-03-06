//
// Created by ssheikh on 2021-02-27.
//

#ifndef RUNE_INCLUDE_HEAD_HANDLER_HH
#define RUNE_INCLUDE_HEAD_HANDLER_HH

#include <mithril/channel/channel_handler.hh>
#include <mithril/channel/channel_inbound_handler.hh>
#include <mithril/channel/channel_outbound_handler.hh>
#include <mithril/channel/channel_handler_context.hh>
#include <mithril/message.hh>

#include <memory>
#include <utility>

class channel;

class head_handler final
    : public channel_inbound_handler_adapter, public channel_outbound_handler
{
private:
  std::weak_ptr<channel> m_channel;
public:
  explicit head_handler(std::weak_ptr<channel> ch)
      : m_channel(std::move(ch))
  {
  }

  void channel_active(channel_handler_context& ctx) final;

  void channel_inactive(channel_handler_context& ctx) final;

  void channel_read(channel_handler_context&, mithril::message) final;

  void write(channel_handler_context&, mithril::message) final;

  channel_direction direction() final
  {
    return channel_direction::outbound;
  }
};

#endif //RUNE_INCLUDE_HEAD_HANDLER_HH
