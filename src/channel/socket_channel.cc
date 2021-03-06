//
// Created by ssheikh on 2021-03-04.
//

#include <mithril/channel/socket_channel.hh>

void socket_channel::write(mithril::message msg)
{
  if (!msg->holds<byte_buffer>()) {
    RUNE_LOG(warning) << "Unhandled write...";
    return;
  }

  tx.emplace(output.write(std::move(msg)->unwrap<byte_buffer>()).then([this] {
    return output.flush();
  }).handle_exception([](std::exception_ptr e) {
    RUNE_LOG(warning) << "Exceptional write!";
  }));
}

seastar::future<> socket_channel::dispatch_all_tx()
{
  return seastar::async([this] {
    while (!tx.empty()) {
      auto future = std::move(tx.front());
      future.get();
      tx.pop();
    }
  });
}
