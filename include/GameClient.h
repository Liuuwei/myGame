//
// Created by Liuwei on 2023/9/4.
//

#ifndef GAME_GAMECLIENT_H
#define GAME_GAMECLIENT_H

#include <ncurses.h>
#include <semaphore>

#include <my/EventLoop.h>
#include <my/TcpClient.h>
#include <my/Log.h>

#include "Player.h"
#include "NonPlayer.h"

class GameClient {
private:

public:
    GameClient(EventLoop* loop, const std::string& ip, int port);
    ~GameClient();
    void start();
private:
    EventLoop* loop_;
    TcpClient client_;
    Player* myself_;
    WINDOW* win;
    void onConnection(const std::shared_ptr<TcpConnection>&);
    void onMessage(const std::shared_ptr<TcpConnection>&, Buffer*);
    void send(char op);
    void show(const Player& player);
    void move();
    std::pair<int, int> parse(const std::string&);
    std::vector<std::vector<char>> map_;
    std::unordered_map<int, Player> allPlayers_;
    std::binary_semaphore start_;
    std::vector<WINDOW *> windows_;
};

#endif //GAME_GAMECLIENT_H
