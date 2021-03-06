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

#include <mithril/bootstrap/client_event_loop_service.hh>

using seastar::stop_iteration;

seastar::future<> client_event_loop_service::handle_connection(seastar::connected_socket socket)
{
  channel = std::make_shared<socket_channel>(std::move(socket));
  channel->attach_pipeline(std::make_shared<channel_pipeline>());
  channel->pipeline()->add_last("init", bs.initializer);
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

void client_event_loop_service::start()
{
  MITHRIL_LOG(info) << "Started client...";

  seastar::socket_address local = seastar::socket_address(::sockaddr_in {AF_INET, INADDR_ANY, {0}});
  seastar::ipv4_addr endpoint;
  try {
    endpoint = seastar::ipv4_addr(bs.host, bs.port);
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

seastar::future<> client_event_loop_service::stop()
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

client_event_loop_service::client_event_loop_service(mithril::bootstrap bs)
    : bs(std::move(bs))
{
}
