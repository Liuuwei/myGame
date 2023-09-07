//
// Created by Liuwei on 2023/9/4.
//
#include "GameServer.h"

int main() {
    Log::Instance()->close();
    EventLoop loop;
    InetAddr addr("10.211.55.3", 7777);
    GameServer server(&loop, addr);
    server.start();
    loop.loop();
}