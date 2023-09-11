//
// Created by Liuwei on 2023/9/4.
//
#include "GameClient.h"

int main() {
    EventLoop loop;
    GameClient client(&loop, "123.60.6.179", 7777);
    client.start();
    loop.loop();
}