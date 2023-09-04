//
// Created by Liuwei on 2023/9/3.
//
#include "GameServer.h"

GameServer::GameServer(EventLoop *loop, const InetAddr& addr) :loop_(loop), server_(loop_, addr),
                        tags_(26), marks_(0), lines_(30), cols_(50) {
    for (int i = 0; i < 26; i++) {
        tags_[i] = i + 'a';
    }
}

void GameServer::start() {
    server_.setThreadNums(1);
    server_.setOnMessage([this](auto && PH1, auto && PH2) {
        onMessage(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
    server_.setOnConnection([this](auto && PH1) {
        onConnection(std::forward<decltype(PH1)>(PH1));
    });
    server_.start();
}

void GameServer::onConnection(const std::shared_ptr<TcpConnection> &conn) {
    char tag = newPlayer();
    conn->send("TAG" + std::string(1, tag) + "\r\n\r\n");
    auto allPlayer = conn->loop()->tcpConnections();
    for (const auto& it : allPlayer) {
        auto player = it.second.lock();
        if (player) {
            if (player->fd() != conn->fd())
                player->send(std::string(1, tag) + "1:1\r\n\r\n");
        }
    }
}

void GameServer::onMessage(const std::shared_ptr<TcpConnection> &conn, Buffer *buffer) {
    int n = buffer->FindEnd();
    if (n != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        auto tag = msg[0];
        auto op = msg[1];
        auto& curPlayer = players_[tag];
        if (move(curPlayer, op)) {
            std::string str = curPlayer.tag() + std::to_string(curPlayer.x()) + ":" + std::to_string(curPlayer.y()) + "\r\n\r\n";
            auto& allPlayer = conn->loop()->tcpConnections();
            for (const auto& it : allPlayer) {
                auto player = it.second.lock();
                if (player) {
                    player->send(str);
                }
            }
        } else {
            std::cout << tag << ": move failed" << std::endl;
        }
    }
}

char GameServer::newPlayer() {
    char tag = tags_[marks_];
    marks_ = (marks_ + 1) % tags_.size();
    players_[tag] = Player(1, 1, tag);
    return tag;
}

bool GameServer::move(Player& player, char op) const{
    return player.move(op);
}