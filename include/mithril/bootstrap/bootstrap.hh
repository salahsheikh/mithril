//
// Created by ssheikh on 2021-02-27.
//

#ifndef MITHRIL_INCLUDE_BOOTSTRAP_HH
#define MITHRIL_INCLUDE_BOOTSTRAP_HH

#include <mithril/bootstrap/server_event_loop_service.hh>
#include <mithril/bootstrap/client_event_loop_service.hh>
#include <mithril/common.hh>

#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>

#include <memory>
#include <string>

class channel_handler;

namespace mithril {
/// TODO: Handle SSL
class bootstrap
{
private:
  std::string host;
  unsigned short port = {0};
public:
  bootstrap() = default;

  bool validate()
  {
    if (port > 0)
      return true;

    return false;
  }

  bootstrap& bind(unsigned short provided_port)
  {
    bootstrap::port = provided_port;
    return *this;
  }

  bootstrap& connect(std::string provided_host, unsigned short provided_port)
  {
    bootstrap::host = std::move(provided_host);
    bootstrap::port = provided_port;
    return *this;
  }

  template<class initializer>
  void start_client(int argc, char** argv)
  {
    if (!validate() && !host.empty())
      throw std::exception();

    seastar::app_template app;
    seastar::distributed<client_event_loop_service<initializer>> client;

    app.run_deprecated(argc, argv, [&] {
      seastar::engine().at_exit([&] {
        return client.stop();
      });

      return client.start_single(host, port).then([&client] {
        return client.invoke_on(0, &client_event_loop_service<initializer>::start);
      });
    });
  }

  template<class initializer>
  void start_server(int argc, char** argv)
  {
    if (!validate())
      throw std::exception();

    seastar::app_template app;
    seastar::distributed<server_event_loop_service<initializer>> server;

    app.run_deprecated(argc, argv, [&] {
      seastar::engine().at_exit([&] {
        return server.stop();
      });

      return server.start(port).then([&] {
        return server.invoke_on_all(&server_event_loop_service<initializer>::start).handle_exception([](auto) {
          MITHRIL_LOG(fatal) << "Server startup failed";
        });
      });
    });
  }

};

}

#endif //MITHRIL_INCLUDE_BOOTSTRAP_HH
