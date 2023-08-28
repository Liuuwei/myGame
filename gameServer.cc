//
// Created by Liuwei on 2023/8/28.
//
#include <my/TcpServer.h>
#include <my/EventLoop.h>
#include <my/Log.h>

void readCallback(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer) {
    int n = buffer->FindEnd();
    if (n != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        std::cout << msg << std::endl;
        auto all = conn->loop()->TcpS();
        for (auto& it : all) {
            if (it.first != conn->fd())
                it.second->send(msg);
        }
    }
}

int main() {
    Log::Instance()->close();
    InetAddr addr("127.0.0.1", 9999);
    EventLoop loop;
    TcpServer server(&loop, addr);
    server.setOnMessageCallback(readCallback);
    server.setThreadNums(1);
    server.start();
    loop.loop();
}