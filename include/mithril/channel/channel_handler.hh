//
// Created by ssheikh on 2021-02-25.
//

#ifndef MITHRIL_INCLUDE_CHANNEL_HANDLER_HH
#define MITHRIL_INCLUDE_CHANNEL_HANDLER_HH

enum class channel_direction
{
  inbound, outbound, bidirectional
};

class channel_handler_context;

class channel_handler
{
public:
  virtual void handler_added(channel_handler_context&) = 0;

  virtual void handler_removed(channel_handler_context&) = 0;

  virtual channel_direction direction() = 0;

  virtual ~channel_handler() = default;
};

class channel_handler_adapter
    : public virtual channel_handler
{
public:
  void handler_added(channel_handler_context&) override
  {
  }

  void handler_removed(channel_handler_context&) override
  {
  }
};

#endif //MITHRIL_INCLUDE_CHANNEL_HANDLER_HH
