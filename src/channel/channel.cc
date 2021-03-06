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

#include <rune/channel/channel.hh>

#include <rune/channel/head_handler.hh>
#include <rune/channel/tail_handler.hh>

void channel::attach_pipeline(std::shared_ptr<channel_pipeline> pipeline)
{
  using std::make_shared;
  using std::move;

  auto head_handler_instance = make_shared<head_handler>(shared_from_this());
  auto head_ctx = make_shared<default_channel_handler_context>(pipeline, move(head_handler_instance));
  head_ctx->handler().handler_added(*head_ctx);

  auto tail_handler_instance = make_shared<tail_handler>();
  auto tail_ctx = make_shared<default_channel_handler_context>(pipeline, move(tail_handler_instance));
  tail_ctx->handler().handler_added(*tail_ctx);

  head_ctx->set_next(tail_ctx);
  tail_ctx->set_prev(head_ctx);

  pipeline->contexts.emplace_back("head", head_ctx);
  pipeline->contexts.emplace_back("tail", tail_ctx);

  m_pipeline = move(pipeline);
}