//
// Created by ssheikh on 2021-02-27.
//

#ifndef RUNE_INCLUDE_BOOTSTRAP_HH
#define RUNE_INCLUDE_BOOTSTRAP_HH

#include <memory>
#include <string>

class channel_handler;

namespace mithril {
class server_event_loop_service;
}
class client_event_loop_service;

namespace mithril {
class bootstrap
{
private:
  std::shared_ptr<channel_handler> initializer;
  std::string host;
  unsigned short port = {0};
public:
  bootstrap() = default;

  bool validate()
  {
    if (initializer && port > 0)
      return true;

    return false;
  }

  template<class channel_initializer>
  bootstrap& handler()
  {
    initializer = std::make_shared<channel_initializer>();
    return *this;
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

  void start_client(int argc, char** argv);

  void start_server(int argc, char** argv);

  friend server_event_loop_service;
  friend client_event_loop_service;

};

}

#endif //RUNE_INCLUDE_BOOTSTRAP_HH
