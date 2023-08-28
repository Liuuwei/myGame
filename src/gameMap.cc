//
// Created by Liuwei on 2023/8/28.
//
#include "gameMap.h"

#include <iostream>
#include <my/Log.h>

gameMap::gameMap() : lines_(0), cols_(0), x_(1), y_(1), score_(0), tcpClient_(&loop_, "127.0.0.1", 9999), peerX_(1), peerY_(1) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    lines_ = LINES / 3 * 2;
    cols_ = COLS / 3 * 2;
    win = newwin(lines_, cols_, 0, 0);
    map_.resize(lines_);
    for (auto& s : map_) {
        s.resize(cols_, ' ');
    }
    tcpClient_.setReadCallback(std::bind(&gameMap::readCallback, this, std::placeholders::_1, std::placeholders::_2));
    tcpClient_.setWriteCallback(std::bind(&gameMap::writeCallback, this, std::placeholders::_1, std::placeholders::_2));
    tcpClient_.connect();
}

gameMap::~gameMap() {
    endwin();
}

void gameMap::show() {
    for (int i = 1; i < lines_; i++) {
        for (int j = 1; j < cols_; j++) {
            if (i == x_ && j == y_) continue;
            mvwaddstr(win, i, 0 , map_[i].c_str());
        }
    }
    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

void gameMap::move() {
    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        if (!check(ch)) {
            continue;
        }
        map_[x_][y_] = ' ';
        if (ch == 'w') {
            x_--;
        } else if (ch == 's') {
            x_++;
        } else if (ch == 'a') {
            y_--;
        } else if (ch == 'd') {
            y_++;
        }
        if (map_[x_][y_] == '.') score_++;
        map_[x_][y_] = '+';
        tcpClient_.send(std::to_string(x_) + "a" + std::to_string(y_) + "a" + "\r\n\r\n");
        show();
    }
}

bool gameMap::check(int ch) {
    if (ch == 'w') {
        if (x_ - 1 < 1 || locChar(x_ - 1, y_) == '#') {
            return false;
        }
    } else if (ch == 's') {
        if (x_ + 1 >= lines_ - 1 || locChar(x_ + 1, y_) == '#') {
            return false;
        }
    } else if (ch == 'a') {
        if (y_ - 1 < 1 || locChar(x_, y_ - 1) == '#') {
            return false;
        }
    } else if (ch == 'd') {
        if (y_ + 1 >= cols_ - 1 || locChar(x_, y_ + 1) == '#') {
            return false;
        }
    }
    return true;
}

void gameMap::setChar(int x, int y, char c) {
    if (x < 0 || x >= lines_ || y < 0 || y >= cols_) return;
    map_[x][y] = c;
}

void gameMap::readCallback(const std::shared_ptr<TcpConnection> &conn, Buffer *buffer) {
    int n = buffer->FindEnd();
    Log::Instance()->LOG("trigger readCallback");
    if (n != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        auto [x, y] = parseLocation(msg);
        Log::Instance()->LOG("move to %d %d", x, y);
        map_[peerX_][peerY_] = ' ';
        peerX_ = x, peerY_ = y;
        map_[peerX_][peerY_] = 'x';
        show();
    }
}

std::pair<int, int> gameMap::parseLocation(const std::string &msg) {
    std::string x, y;
    int i = 0;
    while (i < msg.size() && msg[i] != 'a') x += msg[i++];
    i += 1;
    while (i < msg.size() && msg[i] != 'a') y += msg[i++];
    int X = std::stoi(x), Y = std::stoi(y);
    return {X, Y};
}

void gameMap::writeCallback(const std::shared_ptr<TcpConnection> &conn, Buffer *buffer) {
    return;
}