//
// Created by Liuwei on 2023/8/28.
//

#ifndef GAME_GAMEMAP_H
#define GAME_GAMEMAP_H

#include <vector>
#include <string>
#include <ncurses.h>

#include <my/TcpClient.h>
#include <my/EventLoop.h>

class gameMap {
public:
    gameMap();
    ~gameMap();
    void show();
    void move();
    void setChar(int x, int y, char c);
    void readCallback(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer);
    void writeCallback(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer);
private:
    std::vector<std::string> map_;
    int lines_;
    int cols_;
    int x_;
    int y_;
    int peerX_;
    int peerY_;
    WINDOW* win;
    char locChar(int x, int y) const { return map_[x][y]; }
    bool check(int ch);
    int score_;
    EventLoop loop_;
    TcpClient tcpClient_;
    std::pair<int, int> parseLocation(const std::string& msg);
};

#endif //GAME_GAMEMAP_H
