//
// Created by ssheikh on 2021-02-27.
//

#ifndef MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH
#define MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH

#include <mithril/channel/socket_channel.hh>

#include <seastar/core/future.hh>
#include <seastar/net/api.hh>
#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/distributed.hh>

#include <optional>

using seastar::stop_iteration;

template<typename T>
class client_event_loop_service
{
private:
  std::optional<seastar::future<>> task;
  std::shared_ptr<socket_channel> channel;
  bool running = false;

  unsigned short port;
  std::string host;
public:

  explicit client_event_loop_service(std::string_view host, unsigned short PORT)
      : port {PORT}, host {host}
  {
  }

  void start()
  {
    MITHRIL_LOG(info) << "Started client...";

    seastar::socket_address local = seastar::socket_address(::sockaddr_in {AF_INET, INADDR_ANY, {0}});
    seastar::ipv4_addr endpoint;
    try {
      endpoint = seastar::ipv4_addr(host, port);
    } catch (std::exception& e) {
      MITHRIL_LOG(fatal) << "Invalid host and port combination";
      seastar::engine_exit(std::make_exception_ptr(e));
      return;
    }

    auto socket = seastar::connect(endpoint, local, seastar::transport::TCP);

    task = socket.then([this](seastar::connected_socket connection) {
      running = true;
      return handle_connection(std::move(connection));
    }).handle_exception([](std::exception_ptr ep) {
      MITHRIL_LOG(info) << "Encountered exception: " << ep;
      seastar::engine_exit(ep);
    });
  }

  seastar::future<> handle_connection(seastar::connected_socket socket)
  {
    channel = std::make_shared<socket_channel>(std::move(socket));
    channel->attach_pipeline(std::make_shared<channel_pipeline>());
    channel->pipeline()->add_last("init", std::make_shared<T>());
    channel->pipeline()->remove("init");
    channel->pipeline()->fire_channel_active();

    auto fire_disconnect = [this] {
      return channel->dispatch_all_tx().then([this] {
        return channel->output.close();
      }).then([this] {
        channel->pipeline()->fire_channel_inactive();
        running = false;
        seastar::engine_exit();
        return seastar::make_ready_future<>();
      });
    };

    return seastar::repeat([this] {
      return channel->input.read().then([this](auto buf) {
        if (buf) {
          channel->pipeline()->fire_channel_read(mithril::create_message(std::move(buf)));
          return seastar::make_ready_future<stop_iteration>(stop_iteration::no);
        } else {
          return seastar::make_ready_future<stop_iteration>(stop_iteration::yes);
        }
      });
    }).then(fire_disconnect).handle_exception([](auto e) {
      MITHRIL_LOG(warning) << "Exception event in service on shard: " << seastar::this_shard_id();
      MITHRIL_LOG(warning) << e;
    });
  }

  seastar::future<> stop()
  {
    if (running) {
      running = false;
      channel->socket().shutdown_input();
      channel->socket().shutdown_output();

      return task->finally([] {
        return seastar::make_ready_future<>();
      });
    }
    return seastar::make_ready_future<>();
  }

};

#endif //MITHRIL_INCLUDE_MITHRIL_CLIENT_EVENT_LOOP_SERVICE_HH
