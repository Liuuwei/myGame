//
// Created by Liuwei on 2023/9/4.
//
#include "NonPlayer.h"

NonPlayer::NonPlayer() : Player(1, 1, '.', 1, 1) {

}

NonPlayer::NonPlayer(int x, int y) : Player(x, y, '.', x, y) {

}