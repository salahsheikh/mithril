//
// Created by ssheikh on 2021-02-27.
//

#ifndef MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH
#define MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH

#include <mithril/bootstrap/bootstrap.hh>
#include <mithril/channel/socket_channel.hh>

#include <seastar/core/future.hh>
#include <seastar/net/api.hh>
#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/distributed.hh>

#include <optional>

namespace mithril {
class bootstrap;
}

class client_event_loop_service
{
private:
  std::optional<seastar::future<>> task;
  std::shared_ptr<socket_channel> channel;
  mithril::bootstrap bs;
  bool running = false;
public:
  explicit client_event_loop_service(mithril::bootstrap bs);

  seastar::future<> handle_connection(seastar::connected_socket connection);

  void start();

  seastar::future<> stop();
};

#endif //MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH
