//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_HANDLER_CONTEXT_HH
#define RUNE_INCLUDE_CHANNEL_HANDLER_CONTEXT_HH

#include "rune/channel/channel_handler.hh"
#include "rune/channel/channel_inbound_invoker.hh"
#include "rune/channel/channel_inbound_handler.hh"

#include "rune/channel/channel_outbound_invoker.hh"
#include "rune/channel/channel_outbound_handler.hh"
#include "rune/common.hh"
#include <rune/message.hh>

#include <seastar/core/future.hh>

#include <memory>
#include <utility>

class channel_pipeline;

class channel_handler_context
    : public channel_inbound_invoker, public channel_outbound_invoker
{
protected:
  std::weak_ptr<channel_handler_context> prev, next;
private:
  std::weak_ptr<channel_pipeline> m_pipeline;
public:

  channel_handler_context(std::weak_ptr<channel_pipeline> pipeline)
      : m_pipeline(std::move(pipeline))
  {

  }

  void set_prev(std::weak_ptr<channel_handler_context> context)
  {
    prev = std::move(context);
  }

  void set_next(std::weak_ptr<channel_handler_context> context)
  {
    next = std::move(context);
  }

  auto get_prev()
  {
    return prev;
  }

  auto get_next()
  {
    return next;
  }

  channel_handler_context* find_context_inbound();

  channel_handler_context* find_context_outbound();

  virtual void invoke_channel_active() = 0;

  virtual void invoke_channel_inactive() = 0;

  virtual void invoke_channel_read(rune::message msg) = 0;

  virtual void invoke_write(rune::message msg) = 0;

  virtual channel_direction direction() = 0;

  virtual channel_handler& handler() = 0;

  virtual channel_handler_context& read() = 0;

  virtual void write(rune::message msg) = 0;

  template<typename ValueType>
  void write(ValueType&& v)
  {
    write(rune::create_message(std::forward<ValueType>(v)));
  }

  channel_pipeline& pipeline()
  {
    return *m_pipeline.lock();
  }

  friend channel_pipeline;
};

class default_channel_handler_context
    : public channel_handler_context
{
  std::shared_ptr<channel_handler> m_handler;
public:
  explicit default_channel_handler_context(std::weak_ptr<channel_pipeline> pipeline,
                                           std::shared_ptr<channel_handler> handler)
      : channel_handler_context(std::move(pipeline)), m_handler(std::move(handler))
  {
  }

  void fire_channel_active() override
  {
    find_context_inbound()->invoke_channel_active();
  }

  void fire_channel_inactive() override
  {
    find_context_inbound()->invoke_channel_inactive();
  }

  void fire_channel_read(rune::message msg) override
  {
    find_context_inbound()->invoke_channel_read(std::move(msg));
  }

  channel_handler& handler() override
  {
    return *m_handler;
  }

  channel_direction direction() override
  {
    return m_handler->direction();
  }

  void invoke_channel_active() override
  {
    as_inbound(m_handler)->channel_active(*this);
  }

  void invoke_channel_inactive() override
  {
    as_inbound(m_handler)->channel_inactive(*this);
  }

  void invoke_channel_read(rune::message msg) override
  {
    as_inbound(m_handler)->channel_read(*this, std::move(msg));
  }

  default_channel_handler_context& read() override
  {
    return *this;
  }

  void invoke_write(rune::message msg) override
  {
    as_outbound(m_handler)->write(*this, std::move(msg));
  }

  void write(rune::message msg) override
  {
    find_context_outbound()->invoke_write(std::move(msg));
  }

private:
  static std::shared_ptr<channel_inbound_handler> as_inbound(std::shared_ptr<channel_handler>& handler)
  {
    DCHECK(handler);
    return std::dynamic_pointer_cast<channel_inbound_handler>(handler);
  }

  static std::shared_ptr<channel_outbound_handler> as_outbound(std::shared_ptr<channel_handler>& handler)
  {
    DCHECK(handler);
    return std::dynamic_pointer_cast<channel_outbound_handler>(handler);
  }
};

#endif //RUNE_INCLUDE_CHANNEL_HANDLER_CONTEXT_HH
