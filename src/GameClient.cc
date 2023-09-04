//
// Created by Liuwei on 2023/9/4.
//
#include "GameClient.h"
#include "Player.h"
#include "NonPlayer.h"

GameClient::GameClient(EventLoop *loop, const std::string &ip, int port) : loop_(loop), client_(loop_, ip, port),
                                                                            myself_(nullptr),
                                                                            map_(DEFAULT_LINES, std::vector<char>(DEFAULT_COLS, ' ')),
                                                                            win(nullptr) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    win = newwin(DEFAULT_LINES, DEFAULT_COLS, 0, 0);
}

GameClient::~GameClient() {
    endwin();
}

void GameClient::start() {
    client_.setOnConnection([this](auto && PH1) {
        onConnection(std::forward<decltype(PH1)>(PH1));
    });
    client_.setReadCallback([this](auto && PH1, auto && PH2) {
        onMessage(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
    client_.closeNagle();
    client_.connect();
}

void GameClient::onConnection(const std::shared_ptr<TcpConnection> &conn) {
    show();
    move();
//    int i = 0;
//    char op[4] = {'w', 's', 'a', 'd'};
//    while (true) {
//        sleep(1);
//        char c = op[i];
//        i = (i + 1) % 4;
//        std::cout << "send " << std::endl;
//        conn->send(std::string(1, 'a') + c + "\r\n\r\n");
//    }
}

void GameClient::onMessage(const std::shared_ptr<TcpConnection> & conn, Buffer *buffer) {
    int n = -1;
    while ( (n = buffer->FindEnd()) != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        if (msg[0] == 'T') {
            auto tag = msg[3];
            myself_ = new Player(1, 1, tag);
            map_[myself_->x()][myself_->y()] = tag;
        } else {
            auto tag = msg[0];
            auto [x, y] = parse(msg);
            if (tag == myself_->tag()) {
                map_[myself_->x()][myself_->y()] = ' ';
                myself_->setX(x);
                myself_->setY(y);
                map_[myself_->x()][myself_->y()] = myself_->tag();
            } else {
                if (otherPlayers_.find(tag) != otherPlayers_.end()) { // 如果不是新玩家，需要先清除原本位置
                    map_[otherPlayers_[tag].x()][otherPlayers_[tag].y()] = ' ';
                    otherPlayers_[tag].setX(x);
                    otherPlayers_[tag].setY(y);
                } else {
                    otherPlayers_[tag] = Player(x, y, tag);
                }
                map_[otherPlayers_[tag].x()][otherPlayers_[tag].y()] = tag;
            }
        }
        show();
    }
}

void GameClient::show() {
    for (int i = 0; i < map_.size(); i++) {
        for (int j = 0; j < map_[0].size(); j++) {
            mvwaddch(win, i, j , map_[i][j]);
        }
    }
    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

void GameClient::move() {
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
        } else {
            continue;
        }
        client_.send(myself_->tag() + op + "\r\n\r\n");
    }
}

std::pair<int, int> GameClient::parse(const std::string & msg) {
    int x = 0, y = 0;
    int i = 1;
    while (i < msg.size() && msg[i] != ':') {
        x = x * 10 + msg[i] - '0';
        i++;
    }
    i++;
    while (i < msg.size() && msg[i] != '\r') {
        y = y * 10 + msg[i] - '0';
        i++;
    }
    return {x, y};
}