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

#include <rune/channel/channel_handler_context.hh>

channel_handler_context* channel_handler_context::find_context_inbound()
{
  channel_handler_context* ctx = this;
  do {
    if (auto next_ctx = ctx->next.lock()) {
      ctx = next_ctx.get();
    } else {
      break;
    }
  } while (ctx->direction() != channel_direction::inbound);

  return ctx;
}

channel_handler_context* channel_handler_context::find_context_outbound()
{
  channel_handler_context* ctx = this;
  do {
    if (auto next_ctx = ctx->prev.lock()) {
      ctx = next_ctx.get();
    } else {
      break;
    }
  } while (ctx->direction() != channel_direction::outbound);

  return ctx;
}
