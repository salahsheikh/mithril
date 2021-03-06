//
// Created by ssheikh on 2021-02-27.
//

#ifndef RUNE_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH
#define RUNE_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH

#include <rune/channel/socket_channel.h>
#include <rune/channel/channel_pipeline.hh>
#include <rune/channel/channel_initializer.hh>
#include <rune/bootstrap/bootstrap.hh>

#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/future-util.hh>
#include <seastar/net/api.hh>
#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/thread.hh>

#include <utility>

namespace rune {
class server_event_loop_service
{
private:
  std::optional<seastar::future<>> task;
  seastar::server_socket server_socket;
  bool running = false;
  bootstrap bs;

public:

  explicit server_event_loop_service(bootstrap bs)
      : bs(std::move(bs))
  {
  }

  seastar::future<> handle_connection(seastar::connected_socket socket, seastar::socket_address remote_address);

  seastar::future<> start();

  seastar::future<> stop();
};

}

#endif //RUNE_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH
