//
// Created by Liuwei on 2023/9/3.
//

#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <set>

#include <my/EventLoop.h>
#include <my/TcpServer.h>
#include <my/Log.h>

#include "Player.h"
#include "NonPlayer.h"

class GameServer {
public:
    GameServer(EventLoop* loop, const InetAddr& addr);
    void start();
private:
    EventLoop* loop_;
    TcpServer server_;
    int lines_;
    int cols_;
    void onConnection(const std::shared_ptr<TcpConnection>&);
    void onMessage(const std::shared_ptr<TcpConnection>&, Buffer*);
    bool move(Player&, char) const;
    char newPlayer();
    size_t marks_;
    std::vector<char> tags_;
    std::unordered_map<char, Player> players_;
    std::set<NonPlayer> nonPlayers_;
};

#endif //GAME_GAMESERVER_H
