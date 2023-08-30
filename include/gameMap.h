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
    void setClient(TcpClient* client) { client_ = client; }
    void handler(const std::string& msg);
    void setTag(char tag) { tag_ = tag; }
private:
    std::vector<std::string> map_;
    int lines_;
    int cols_;
    int x_;
    int y_;
    int peerX_;
    int peerY_;
    TcpClient* client_;
    WINDOW* win;
    void handlerEnd();
    void handlerEnd(const std::string& msg);
    void clearLoc();
    void updateLoc();
    void handlerGenerate(int x, int y);
    char locChar(int x, int y) const { return map_[x][y]; }
    bool check(int ch);
    int score_;
    int peerScore_;
    EventLoop loop_;
    char tag_;
    std::pair<int, int> parse(const std::string& msg);
    timeval time_;
    long long curTime_;
};

#endif //GAME_GAMEMAP_H
