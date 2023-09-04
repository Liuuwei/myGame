//
// Created by Liuwei on 2023/9/4.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

extern const int DEFAULT_LINES;
extern const int DEFAULT_COLS;

class Player {
public:
    Player();
    Player(int x, int y, char tag);
    Player(int x, int y, char tag, int lines, int cols);
    [[nodiscard]] int x() const { return x_; }
    [[nodiscard]] int y() const { return y_; }
    [[nodiscard]] char tag() const { return tag_; }
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    bool move(char op);
private:
    int x_;
    int y_;
    char tag_;
    int up_;
    int down_;
    int left_;
    int right_;
};

#endif //GAME_PLAYER_H