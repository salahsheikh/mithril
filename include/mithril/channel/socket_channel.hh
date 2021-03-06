//
// Created by ssheikh on 2021-02-25.
//

#ifndef RUNE_INCLUDE_SERVER_CHANNEL_H
#define RUNE_INCLUDE_SERVER_CHANNEL_H

#include <mithril/common.hh>
#include <mithril/channel/channel.hh>
#include <mithril/bootstrap/server_event_loop_service.hh>
#include <mithril/message.hh>
#include <mithril/buffer/byte_buffer.hh>

#include <seastar/net/api.hh>
#include <seastar/core/thread.hh>

#include <queue>

class socket_channel final
    : public channel
{
private:
  seastar::connected_socket m_socket;
  seastar::socket_address m_address;
public:
  std::queue<seastar::future<>> tx;

  seastar::input_stream<char> input;
  seastar::output_stream<char> output;

  explicit socket_channel(seastar::connected_socket&& socket)
      : m_socket(std::move(socket))
  {
    input = std::move(m_socket.input());
    output = std::move(m_socket.output());
  }

  void set_address(seastar::socket_address address)
  {
    m_address = address;
  }

  socket_channel& read() override
  {
    return *this;
  }

  seastar::future<> dispatch_all_tx();

  void write(mithril::message msg) override;

  seastar::socket_address address() const
  {
    return m_address;
  }

  seastar::connected_socket& socket()
  {
    return m_socket;
  }

};

#endif //RUNE_INCLUDE_SERVER_CHANNEL_H
