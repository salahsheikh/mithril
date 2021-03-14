# mithril

A C++17 async/non-blocking networking client-server framework inspired by [Netty](https://netty.io/) and built on top
of [scylladb/seastar](https://github.com/scylladb/seastar).

Special attention was given to:

* Compose I/O operations in a structured manner to avoid "callback hell"
* Scaling well with additional cores
* Minimizing memory copies

### Examples

* [Echo server and client](https://github.com/salahsheikh/mithril/tree/main/examples/echo)

### See also

[facebook/wangle](https://github.com/facebook/wangle)