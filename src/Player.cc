//
// Created by Liuwei on 2023/9/4.
//
#include "Player.h"

const int DEFAULT_LINES = 30;
const int DEFAULT_COLS = 50;

Player::Player() : x_(1), y_(1), tag_('0'), up_(1), down_(DEFAULT_LINES), left_(1), right_(DEFAULT_COLS) {

}

Player::Player(int x, int y, char tag) : x_(x), y_(y), tag_(tag), up_(1), down_(DEFAULT_LINES - 2), left_(1), right_(DEFAULT_COLS - 2) {

}

Player::Player(int x, int y, char tag, int lines, int cols) : x_(x), y_(y), tag_(tag), up_(1), down_(lines - 2), left_(1), right_(cols - 2) {

}

bool Player::move(char op) {
    switch (op) {
        case 'w': {
            if (x_ <= up_) return false;
            x_--;
            break;
        }
        case 's': {
            if (x_ >= down_) return false;
            x_++;
            break;
        }
        case 'a': {
            if (y_ <= left_) return false;
            y_--;
            break;
        }
        case 'd': {
            if (y_ >= right_) return false;
            y_++;
            break;
        }
        default:
            return false;
    }
    return true;
}