#include <rune/bootstrap/bootstrap.hh>
#include <rune/channel/channel_initializer.hh>
#include <rune/channel/channel_pipeline.hh>

#include <iostream>
#include <exception>

#include "echo_server_handler.hh"

static constexpr unsigned short PORT = 43594;

class echo_channel_initializer
    : public channel_initializer
{
public:
  void init_channel(channel_handler_context& ctx) override
  {
    std::cout << "Initialized channel!" << std::endl;
    ctx.pipeline().add_last<echo_server_handler>();
  }
};

int main(int argc, char** argv)
{
  using namespace rune;

  try {
    bootstrap bs;
    bs.handler<echo_channel_initializer>().bind(PORT).start_server(argc, argv);
  } catch (std::exception& e) {
    std::cerr << e.what();
  }
}