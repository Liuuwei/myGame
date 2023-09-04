//
// Created by Liuwei on 2023/9/4.
//

#ifndef GAME_NPC_H
#define GAME_NPC_H

#include "Player.h"

class NonPlayer : public Player{
public:
    NonPlayer();
    NonPlayer(int x, int y);
    bool operator <(const NonPlayer& other) const {
        return x() + y() < other.x() + other.y();
    }
    bool operator ==(const NonPlayer& other) const {
        return x() + y() == other.x() + other.y();
    }
private:
};

#endif //GAME_NPC_H
