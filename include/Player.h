//
// Created by Liuwei on 2023/9/4.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "Util.h"

#include <ncurses.h>

#include <vector>
#include <string>

class Player {
public:
    Player();
    Player(int x, int y, char tag, int window, Model model);
    Player(int x, int y, char tag, int window, Model model, int lines, int cols);
    [[nodiscard]] int x() const { return x_; }
    [[nodiscard]] int y() const { return y_; }
    [[nodiscard]] char tag() const { return tag_; }
    [[nodiscard]] Model model() const { return model_; }
    [[nodiscard]] int window() const { return window_; }
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    void setModel(Model model) { model_ = model; }
    void setTag(char tag) { tag_ = tag; }
    void setWindow(int window) { window_ = window; }
    bool move(char op);
    void show();
    std::string String();
private:
    int x_;
    int y_;
    char tag_; // 玩家符号
    int window_; // 属于哪个窗口
    Model model_; // 当前模式: Insert | Clean | Look
};

#endif //GAME_PLAYER_H