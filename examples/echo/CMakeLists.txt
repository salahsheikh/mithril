add_executable(echo-server echo_server.cc)
target_include_directories(echo-server PRIVATE include)
target_link_libraries(echo-server PRIVATE rune)

add_executable(echo-client echo_client.cc)
target_include_directories(echo-client PRIVATE include)
target_link_libraries(echo-client PRIVATE rune)
