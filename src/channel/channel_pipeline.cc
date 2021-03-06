/*─────────────────────────────────────────────────────────────────────────────┐
│ Copyright 2021 Salah Sheikh                                                  │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFT80WARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
└─────────────────────────────────────────────────────────────────────────────*/

#include <rune/channel/channel_pipeline.hh>
#include <rune/channel/channel.hh>

void channel_pipeline::fire_channel_active()
{
  head()->invoke_channel_active();
}

void channel_pipeline::fire_channel_inactive()
{
  head()->invoke_channel_inactive();
}

void channel_pipeline::fire_channel_read(rune::message msg)
{
  head()->invoke_channel_read(std::move(msg));
}

void channel_pipeline::add_last(std::string_view name, std::shared_ptr<channel_handler> handler)
{
  // Contexts should already hold head and tail handlers.
  DCHECK(!contexts.empty());

  auto new_ctx = std::make_shared<default_channel_handler_context>(shared_from_this(), std::move(handler));
  new_ctx->handler().handler_added(*new_ctx);

  if (auto prev = tail()->get_prev().lock()) {
    new_ctx->set_prev(prev);
    new_ctx->set_next(tail());

    prev->set_next(new_ctx);
    tail()->set_prev(new_ctx);
  }

  contexts.insert(tail_iter(), {name, std::move(new_ctx)});
}

void channel_pipeline::remove(std::string_view name)
{
  auto result = std::find_if(cbegin(contexts), cend(contexts), [&name](const auto& p) {
    return p.first == name;
  });

  if (result != cend(contexts)) {
    if (auto new_prev = result->second->prev.lock()) {
      new_prev->set_next(result->second->next);
    }

    if (auto new_next = result->second->next.lock()) {
      new_next->set_prev(result->second->prev);
    }

    contexts.erase(result);
  }
}
