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

#include <mithril/bootstrap/bootstrap.hh>
#include <mithril/bootstrap/client_event_loop_service.hh>

#include <seastar/core/distributed.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>

namespace mithril {

void bootstrap::start_client(int argc, char** argv)
{
  if (!validate() && !host.empty())
    throw std::exception();

  seastar::app_template app;
  seastar::distributed<client_event_loop_service> client;

  app.run_deprecated(argc, argv, [&] {
    seastar::engine().at_exit([&] {
      return client.stop();
    });

    return client.start_single(*this).then([&client] {
      return client.invoke_on(0, &client_event_loop_service::start);
    });
  });
}

void bootstrap::start_server(int argc, char** argv)
{
  if (!validate())
    throw std::exception();

  seastar::app_template app;
  seastar::distributed<server_event_loop_service> server;

  app.run_deprecated(argc, argv, [&] {
    seastar::engine().at_exit([&] {
      return server.stop();
    });

    return server.start(*this).then([&] {
      return server.invoke_on_all(&server_event_loop_service::start).handle_exception([](auto) {
        MITHRIL_LOG(fatal) << "Server startup failed";
      });
    });
  });
}

}

