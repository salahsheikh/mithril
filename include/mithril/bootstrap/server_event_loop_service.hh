//
// Created by ssheikh on 2021-02-27.
//

#ifndef MITHRIL_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH
#define MITHRIL_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH

#include <mithril/channel/socket_channel.hh>
#include <mithril/channel/channel_pipeline.hh>
#include <mithril/channel/channel_initializer.hh>
#include <mithril/channel/socket_channel.hh>

#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/future-util.hh>
#include <seastar/net/api.hh>
#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/thread.hh>

#include <utility>

using seastar::stop_iteration;

namespace mithril {

template<typename T>
class server_event_loop_service
{
private:
  std::optional<seastar::future<>> task;
  seastar::server_socket server_socket;
  bool running = false;

  unsigned short port;
public:

  explicit server_event_loop_service(unsigned short PORT)
      : port {PORT}
  {
  }

  seastar::future<> handle_connection(seastar::connected_socket socket,
                                                                    seastar::socket_address remote_address)
  {
    auto channel = std::make_shared<socket_channel>(std::move(socket));
    channel->set_address(remote_address);

    return seastar::do_with(channel, [=](auto& channel) {
      channel->attach_pipeline(std::make_shared<channel_pipeline>());
      channel->pipeline()->add_last("init", std::make_shared<T>());
      channel->pipeline()->remove("init");
      channel->pipeline()->fire_channel_active();

      return seastar::repeat([&channel] {
        return channel->input.read().then([&channel](auto buf) {
          if (buf) {
            channel->pipeline()->fire_channel_read(mithril::create_message(std::move(buf)));
            return seastar::make_ready_future<stop_iteration>(stop_iteration::no);
          } else {
            return seastar::make_ready_future<stop_iteration>(stop_iteration::yes);
          }
        });
      }).then([&channel] {
        return channel->dispatch_all_tx().then([&channel] {
          return channel->output.close();
        }).then([&channel] {
          channel->pipeline()->fire_channel_inactive();
          return seastar::make_ready_future<>();
        });
      });
    });
  }

  seastar::future<> start()
  {
    MITHRIL_LOG(trace) << "Starting service on shard: " << seastar::this_shard_id();

    seastar::listen_options lo;
    lo.reuse_address = true;

    try {
      server_socket = seastar::listen(seastar::make_ipv4_address({port}), lo);
    } catch (std::system_error& e) {
      MITHRIL_LOG(fatal) << e.what();
      seastar::engine_exit();
      return seastar::make_exception_future(e);
    }

    running = true;

    auto handle_accept = [this](seastar::accept_result res) {
      (void)handle_connection(std::move(res.connection), res.remote_address).handle_exception([](std::exception_ptr ep) {
        MITHRIL_LOG(warning) << "Could not handle connection: " << ep;
      });
      return seastar::make_ready_future<stop_iteration>(!running ? stop_iteration::yes : stop_iteration::no);
    };

    task = seastar::repeat([this, &handle_accept]() {
      if (!running) {
        return seastar::make_ready_future<stop_iteration>(stop_iteration::yes);
      }
      return server_socket.accept().then(handle_accept);
    }).handle_exception([this](std::exception_ptr ep) {
      if (running) {
        MITHRIL_LOG(fatal) << "Exception event in service on shard: " << seastar::this_shard_id();
        seastar::engine_exit(ep);
      }
    });

    return seastar::make_ready_future<>();
  }

  seastar::future<> stop()
  {
    if (running) {
      running = false;
      server_socket.abort_accept();
      return task->finally([] {
        MITHRIL_LOG(trace) << "Shut down service on shard: " << seastar::this_shard_id();
        return seastar::make_ready_future();
      });
    }
    return seastar::make_ready_future();
  }
};

}

#endif //MITHRIL_INCLUDE_SERVER_EVENT_LOOP_SERVICE_HH
