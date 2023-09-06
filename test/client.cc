//
// Created by Liuwei on 2023/9/4.
//
#include "GameClient.h"

int main() {
    EventLoop loop;
    GameClient client(&loop, "10.211.55.3", 7777);
    client.start();
    loop.loop();
}