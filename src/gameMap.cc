//
// Created by Liuwei on 2023/8/28.
//
#include "gameMap.h"

#include <iostream>
#include <my/Log.h>

gameMap::gameMap() : lines_(0), cols_(0), x_(1), y_(1), score_(0), peerScore_(0), tcpClient_(&loop_, "127.0.0.1", 9999), peerX_(1), peerY_(1) ,
                     time_(), curTime_(0) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    lines_ = 50 / 3 * 2;
    cols_ = 150 / 3 * 2;
    win = newwin(lines_, cols_, 0, 0);
    map_.resize(lines_);
    for (auto& s : map_) {
        s.resize(cols_, ' ');
    }
    tcpClient_.setReadCallback(std::bind(&gameMap::readCallback, this, std::placeholders::_1, std::placeholders::_2));
    tcpClient_.setWriteCallback(std::bind(&gameMap::writeCallback, this, std::placeholders::_1, std::placeholders::_2));
    tcpClient_.connect();
    gettimeofday(&time_, nullptr);
    curTime_ = time_.tv_sec;
}

gameMap::~gameMap() {
    endwin();
}

void gameMap::show() {
    gettimeofday(&time_, nullptr);
    if (time_.tv_sec - curTime_ >= 10) {
        int x = random() % (lines_ - 2) + 1;
        int y = random() % (cols_ - 2) + 1;
        while (map_[x][y] == '#') {
            x = random() % (lines_ - 2) + 1;
            y = random() % (cols_ - 2) + 1;
        }

        std::string msg = "G" + std::to_string(x) + '\r' + std::to_string(y) + "\r\n\r\n";
        handlerGenerate(msg);
        tcpClient_.send(msg);
        curTime_ = time_.tv_sec;
    }
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
        if (map_[x_][y_] == 'x') handlerEnd();
        map_[x_][y_] = '+';
        tcpClient_.send("M" + std::to_string(x_) + "\r" + std::to_string(y_) + "\r\n\r\n");
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
    if (n != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        char op = msg[0];
        switch (op) {
            case 'M' :
                handlerMove(msg);
                break;
            case 'G':
                handlerGenerate(msg);
                break;
            case 'E':
                handlerEnd(msg);
                break;
            default:
                break;
        }
        show();
    }
}

void gameMap::handlerMove(const std::string &msg) {
    auto [x, y] = parse(msg);
    map_[peerX_][peerY_] = ' ';
    peerX_ = x, peerY_ = y;
    if (map_[peerX_][peerY_] == '.') {
        peerScore_++;
    } else if (map_[peerX_][peerY_] == '+') {
        handlerEnd();
    }
    map_[peerX_][peerY_] = 'x';
}

void gameMap::handlerGenerate(const std::string &msg) {
    auto [x, y] = parse(msg);
    map_[x][y] = '.';
}

void gameMap::handlerEnd() {
    if (peerScore_ == score_)
        return;
    if (peerScore_ > score_) {
        tcpClient_.send("EW\r\n\r\n");
        mvwaddstr(win, lines_ / 2, (cols_ - 11) / 2, "YOU LOSE!!!");
    } else if (peerScore_ < score_) {
        tcpClient_.send("EL\r\n\r\n");
        mvwaddstr(win, lines_ / 2, (cols_ - 10) / 2, "YOU WIN!!!");
    }
    wrefresh(win);
    sleep(1);
    exit(0);
}

void gameMap::handlerEnd(const std::string& msg) {
    char c = msg[1];
    if (c == 'L') {
        mvwaddstr(win, lines_ / 2, (cols_ - 11) / 2, "YOU LOSE!!!");
    } else {
        mvwaddstr(win, lines_ / 2, (cols_ - 10) / 2, "YOU WIN!!!");
    }
    wrefresh(win);
    sleep(1);
    exit(0);
}

std::pair<int, int> gameMap::parse(const std::string &msg) {
    std::string x, y;
    int i = 1;
    while (i < msg.size() && msg[i] != '\r') x += msg[i++];
    i += 1;
    while (i < msg.size() && msg[i] != '\r') y += msg[i++];
    int X = std::stoi(x), Y = std::stoi(y);
    return {X, Y};
}

void gameMap::writeCallback(const std::shared_ptr<TcpConnection> &conn, Buffer *buffer) {
    return;
}