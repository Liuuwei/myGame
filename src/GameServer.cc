//
// Created by Liuwei on 2023/9/3.
//
#include "GameServer.h"
#include "Util.h"

GameServer::GameServer(EventLoop *loop, const InetAddr& addr) :loop_(loop), server_(loop_, addr),
                        tags_(26), marks_(0), lines_(30), cols_(50) {

}

void GameServer::start() {
    server_.setThreadNums(1);
    server_.setOnMessage([this](auto && PH1, auto && PH2) {
        onMessage(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
    server_.setOnConnection([this](auto && PH1) {
        onConnection(std::forward<decltype(PH1)>(PH1));
    });
    server_.closeNagle();
    server_.start();
}

void GameServer::onConnection(const std::shared_ptr<TcpConnection> &conn) {
    int window = newPlayer();
    Player newPlayer(1, 1, '.', window, Insert);
    players_[conn->fd()] = newPlayer;
    std::string str = newPlayer.String() + "\r\n\r\n";
    allCommands_.push_back(str);
    for (const auto& command : allCommands_) {
        conn->send(command);
    }

    auto allPlayers = conn->loop()->tcpConnections();
    for (const auto& it : allPlayers) {
        auto player = it.second.lock();
        if (player) {
            player->send(str);
        }
    }
}

void GameServer::onMessage(const std::shared_ptr<TcpConnection> &conn, Buffer *buffer) {
    int n = -1;
    while ( (n = buffer->FindEnd()) != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        std::cout << msg << std::endl;
        char op = msg[0];
        auto& curPlayer = players_[conn->fd()];
        if (move(curPlayer, op)) {
            std::string str = curPlayer.String() + "\r\n\r\n";
            std::cout << str << std::endl;
            auto allPlayers = conn->loop()->tcpConnections();
            allCommands_.push_back(str);
            for (auto& it : allPlayers) {
                auto player = it.second.lock();
                if (player) {
                    player->send(str);
                }
            }
        }
    }
}

int GameServer::newPlayer() {
    static int i = -1;
    i = (i + 1) % 4;
    return i;
}

bool GameServer::move(Player& player, char op) const{
    return player.move(op);
}