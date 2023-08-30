//
// Created by Liuwei on 2023/8/28.
//
#include "gameMap.h"

#include <iostream>
#include <my/Log.h>

gameMap::gameMap() : lines_(0), cols_(0), x_(0), y_(0), score_(0), peerScore_(0), peerX_(0), peerY_(0) ,
                     time_(), curTime_(0), tag_('0'){
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    lines_ = 30;
    cols_ = 30;
    win = newwin(lines_, cols_, 0, 0);
    map_.resize(lines_);
    for (auto& s : map_) {
        s.resize(cols_, ' ');
    }
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

        std::string msg = "G " + std::to_string(x) + ':' + std::to_string(y) + "\r\n\r\n";
        client_->send(msg);
        handlerGenerate(x, y);
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
        std::string op;
        if (ch == 'w') {
            op = "w";
        } else if (ch == 's') {
            op = "s";
        } else if (ch == 'a') {
            op = "a";
        } else if (ch == 'd') {
            op = "d";
        }
        if (tag_)
        client_->send(tag_ + op + "\r\n\r\n");
    }
}

void gameMap::handlerGenerate(int x, int y) {
    map_[x][y] = '.';
}

void gameMap::handlerEnd() {
    if (peerScore_ == score_)
        return;
    if (peerScore_ > score_) {
        client_->send("EW\r\n\r\n");
        mvwaddstr(win, lines_ / 2, (cols_ - 11) / 2, "YOU LOSE!!!");
    } else if (peerScore_ < score_) {
        client_->send("EL\r\n\r\n");
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
    while (i < msg.size() && msg[i] != ':') x += msg[i++];
    i += 1;
    while (i < msg.size() && msg[i] != '\r') y += msg[i++];
    int X = std::stoi(x), Y = std::stoi(y);
    return {X, Y};
}

void gameMap::clearLoc() {
    map_[x_][y_] = map_[peerX_][peerY_] = ' ';
}

void gameMap::updateLoc() {
    map_[x_][y_] = '+';
    map_[peerX_][peerY_] = 'x';
}

void gameMap::handler(const std::string &msg) {
    char tag = msg[0];
    auto [x, y] = parse(msg);
    clearLoc();
    if (tag == tag_) {
        x_ = x;
        y_ = y;
    } else {
        peerX_ = x;
        peerY_ = y;
    }
    updateLoc();
}