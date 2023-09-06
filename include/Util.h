//
// Created by Liuwei on 2023/9/6.
//

#ifndef GAME_UTIL_H
#define GAME_UTIL_H

const int lines = 23;
const int cols = 72;

const int up = 1;
const int down = lines - 2;
const int left = 1;
const int right = cols - 2;

enum Model {
    Look = 1,
    Insert = 2,
    Clean = 3,
};

#endif //GAME_UTIL_H
