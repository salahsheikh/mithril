//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_PIPELINE_HH
#define RUNE_INCLUDE_CHANNEL_PIPELINE_HH

#include "rune/channel/channel_handler_context.hh"
#include "rune/channel/channel_handler.hh"
#include "rune/channel/channel_inbound_handler.hh"
#include "rune/channel/channel_inbound_invoker.hh"
#include "rune/channel/channel_outbound_invoker.hh"
#include "rune/common.hh"
#include <rune/message.hh>

#include <list>
#include <memory>
#include <algorithm>
#include <utility>
#include <string>

class channel;

class channel_pipeline
    : public channel_inbound_invoker,
      public channel_outbound_invoker,
      public std::enable_shared_from_this<channel_pipeline>
{
private:
  std::list<std::pair<const std::string_view, std::shared_ptr<channel_handler_context>>> contexts;

  decltype(contexts)::iterator head_iter()
  {
    return contexts.begin();
  }

  std::shared_ptr<channel_handler_context> head()
  {
    return head_iter()->second;
  }

  decltype(contexts)::iterator tail_iter()
  {
    return --contexts.end();
  }

  std::shared_ptr<channel_handler_context> tail()
  {
    return tail_iter()->second;
  }

public:
  channel_pipeline() = default;

  void fire_channel_active() override;

  void fire_channel_inactive() override;

  void fire_channel_read(rune::message msg) override;

  channel_pipeline& read() override
  {
    return *this;
  }

  void write(rune::message msg) override
  {
    tail()->write(std::move(msg));
  }

  template<class handler, typename... Arg>
  void add_last(std::string_view name = typeid(handler).name(), Arg... args)
  {
    static_assert(std::is_base_of_v<channel_handler, handler>, "Must be derived from channel_handler_context");
    add_last(name, std::make_shared<handler>(args...));
  }

  static std::string_view get_handler_name(const std::shared_ptr<channel_handler>& handler) {
    return std::string_view(typeid(*handler).name());
  }

  void add_last(std::string_view name, std::shared_ptr<channel_handler> handler);

  void remove(std::string_view name);

  ~channel_pipeline()
  {
    for (auto it = contexts.crbegin(); it != contexts.crend(); it++) {
      it->second->handler().handler_removed(*(it->second));
    }
  }

  friend channel;
};

#endif //RUNE_INCLUDE_CHANNEL_PIPELINE_HH
