//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_CHANNEL_HH
#define RUNE_INCLUDE_CHANNEL_HH

#include <rune/channel/channel_pipeline.hh>
#include <rune/channel/channel_outbound_invoker.hh>
#include <rune/message.hh>

#include <seastar/core/future.hh>

class channel
    : public channel_outbound_invoker, public std::enable_shared_from_this<channel>
{
  std::shared_ptr<channel_pipeline> m_pipeline;
public:
  void attach_pipeline(std::shared_ptr<channel_pipeline> pipeline);

  channel& read() override = 0;

  void write(rune::message msg) override = 0;

  std::shared_ptr<channel_pipeline> pipeline()
  {
    return m_pipeline;
  }
};

#endif //RUNE_INCLUDE_CHANNEL_HH
