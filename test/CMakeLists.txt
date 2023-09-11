add_executable(client client.cc)
target_link_libraries(client game)

add_executable(server server.cc)
target_link_libraries(server game)