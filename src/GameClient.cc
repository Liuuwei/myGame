//
// Created by Liuwei on 2023/9/4.
//
#include "GameClient.h"
#include "Player.h"
#include "NonPlayer.h"

GameClient::GameClient(EventLoop *loop, const std::string &ip, int port) : loop_(loop), client_(loop_, ip, port),
                                                                            myself_(nullptr),
                                                                            win(nullptr),
                                                                            start_(0),
                                                                            windows_(4)
                                                                            {
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    int t = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            windows_[t++] = newwin(lines, cols, lines * i, cols * j);
        }
    }
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
    start_.acquire();
    move();
}

void GameClient::onMessage(const std::shared_ptr<TcpConnection> & conn, Buffer *buffer) {
    int n = -1;
    while ( (n = buffer->FindEnd()) != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        auto window = msg[0] - '0';
        auto model = static_cast<Model>(msg[1] - '0');
        auto tag = msg[2];
        auto [x, y] = parse(msg);

        if (allPlayers_.find(window) == allPlayers_.end()) {
            allPlayers_[window] = Player(x, y, '.', window, model);
        }

        auto& player = allPlayers_[window];
        player.setWindow(window);
        player.setModel(model);
        player.setTag(tag);
        player.setX(x);
        player.setY(y);
        show(player);

        static bool first = true;
        if (first) {
            start_.release();
            first = false;
        }
    }
}

void GameClient::show(const Player& player) {
    int window = player.window();
    char tag = player.tag();
    Model model = player.model();
    int x = player.x();
    int y = player.y();

    switch (model) {
        case Insert: {
            mvwaddch(windows_[window], x, y, tag);
            break;
        }
        case Clean: {
            mvwaddch(windows_[window], x, y, ' ');
            break;
        }
        case Look: {
            wmove(windows_[window], x, y);
            break;
        }
        default: {
            break;
        }
    }
    box(windows_[window], ACS_VLINE, ACS_HLINE);
    wrefresh(windows_[window]);
}

void GameClient::move() {
    int key;
    while ((key = getch()) != KEY_F(1)) {
        char op;
        switch (key) {
            case 'i': {
                op = 'i';
                break;
            }
            case 'c': {
                op = 'c';
                break;
            }
            case 'l': {
                op = 'l';
                break;
            }
            case 'w': {
                op = 'w';
                break;
            }
            case 's': {
                op = 's';
                break;
            }
            case 'a': {
                op = 'a';
                break;
            }
            case 'd': {
                op = 'd';
                break;
            }
            default: {
                continue;
            }
        }
        client_.send(std::string(1, op) + "\r\n\r\n");
    }
    exit(0);
}

std::pair<int, int> GameClient::parse(const std::string & msg) {
    int x = 0, y = 0;
    int i = 3;
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

void GameClient::send(char op) {
    int window = myself_->window();
    Model model = myself_->model();
    client_.send(std::to_string(window) + std::to_string(model) + op + "\r\n\r\n");
}