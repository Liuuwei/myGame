//
// Created by Liuwei on 2023/9/4.
//
#include "Player.h"

Player::Player() : x_(1), y_(1), tag_('.') {

}

Player::Player(int x, int y, char tag, int window, Model model) : x_(x), y_(y), tag_(tag),
                                                    window_(window), model_(model) {

}

Player::Player(int x, int y, char tag, int window, Model model, int lines, int cols) : x_(x), y_(y), tag_(tag),
                                                                          window_(window), model_(model) {

}

bool Player::move(char op) {
    switch (op) {
        case 'i': {
            model_ = Insert;
            break;
        }
        case 'c': {
            model_ = Clean;
            break;
        }
        case 'l': {
            model_ = Look;
            break;
        }
        case 'w': {
            if (x_ <= up) return false;
            x_--;
            break;
        }
        case 's': {
            if (x_ >= down) return false;
            x_++;
            break;
        }
        case 'a': {
            if (y_ <= left) return false;
            y_--;
            break;
        }
        case 'd': {
            if (y_ >= right) return false;
            y_++;
            break;
        }
        default:
            return false;
    }
    return true;
}

std::string Player::String() {
    std::string str;
    str.append(std::to_string(window_));
    str.append(std::to_string(model_));
    str.append(std::string(1, tag_));
    str.append(std::to_string(x()));
    str.append(":");
    str.append(std::to_string(y()));
    return str;
}