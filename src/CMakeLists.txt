add_library(game GameServer.cc GameClient.cc Player.cc
        )

target_link_libraries(game tinymuduo ncurses)