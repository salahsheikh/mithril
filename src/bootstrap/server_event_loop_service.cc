/*─────────────────────────────────────────────────────────────────────────────┐
│ Copyright 2021 Salah Sheikh                                                  │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
└─────────────────────────────────────────────────────────────────────────────*/

#include <rune/bootstrap/server_event_loop_service.hh>

using seastar::stop_iteration;

namespace rune {
seastar::future<> server_event_loop_service::handle_connection(seastar::connected_socket socket,
                                                               seastar::socket_address remote_address)
{
  auto channel = std::make_shared<socket_channel>(std::move(socket));
  channel->set_address(remote_address);

  return seastar::do_with(channel, [this](auto& channel) {
    channel->attach_pipeline(std::make_shared<channel_pipeline>());
    const auto initializer_name = channel_pipeline::get_handler_name(bs.initializer);
    channel->pipeline()->add_last(initializer_name, bs.initializer);
    channel->pipeline()->remove(initializer_name);
    channel->pipeline()->fire_channel_active();

    return seastar::repeat([&channel] {
      return channel->input.read().then([&channel](auto buf) {
        if (buf) {
          channel->pipeline()->fire_channel_read(rune::create_message(std::move(buf)));
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

seastar::future<> server_event_loop_service::start()
{
  RUNE_LOG(trace) << "Starting service on shard: " << seastar::this_shard_id();

  seastar::listen_options lo;
  lo.reuse_address = true;

  try {
    server_socket = seastar::listen(seastar::make_ipv4_address({bs.port}), lo);
  } catch (std::system_error& e) {
    RUNE_LOG(fatal) << e.what();
    seastar::engine_exit();
    return seastar::make_exception_future(e);
  }

  auto handle_accept = [this](seastar::accept_result res) {
    (void)handle_connection(std::move(res.connection), res.remote_address).handle_exception([](std::exception_ptr ep) {
      RUNE_LOG(warning) << "Could not handle connection: " << ep;
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
      RUNE_LOG(fatal) << "Exception event in service on shard: " << seastar::this_shard_id();
      seastar::engine_exit(ep);
    }
  });

  running = true;
  return seastar::make_ready_future<>();
}

seastar::future<> server_event_loop_service::stop()
{
  if (running) {
    running = false;
    server_socket.abort_accept();
    return task->finally([] {
      RUNE_LOG(trace) << "Shut down service on shard: " << seastar::this_shard_id();
      return seastar::make_ready_future();
    });
  }
  return seastar::make_ready_future();
}
}
