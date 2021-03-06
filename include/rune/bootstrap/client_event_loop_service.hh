//
// Created by ssheikh on 2021-02-27.
//

#ifndef RUNE_INCLUDE_RUNE_CLIENT_EVENT_LOOP_SERVICE_HH
#define RUNE_INCLUDE_RUNE_CLIENT_EVENT_LOOP_SERVICE_HH

#include <rune/bootstrap/bootstrap.hh>
#include <rune/channel/socket_channel.h>

#include <seastar/core/future.hh>
#include <seastar/net/api.hh>
#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/distributed.hh>

#include <optional>

namespace rune {
class bootstrap;
}

class client_event_loop_service
{
private:
  std::optional<seastar::future<>> task;
  std::shared_ptr<socket_channel> channel;
  rune::bootstrap bs;
  bool running = false;
public:
  explicit client_event_loop_service(rune::bootstrap bs);

  seastar::future<> handle_connection(seastar::connected_socket connection);

  void start();

  seastar::future<> stop();
};

#endif //RUNE_INCLUDE_RUNE_CLIENT_EVENT_LOOP_SERVICE_HH
