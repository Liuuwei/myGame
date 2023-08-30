//
// Created by Liuwei on 2023/8/28.
//
#include <my/TcpServer.h>
#include <my/EventLoop.h>
#include <my/Log.h>

int x, y;
int px, py;

int lines = 30;
int cols = 30;

void handlerMove(char tag, char op) {
    if (tag == 'A') {
        switch (op) {
            case 'w': {
                if (x <= 1) return;
                x -= 1;
                break;
            }
            case 's': {
                if (x >= lines - 2) return;
                x += 1;
                break;
            }
            case 'a': {
                if (y <= 1) return;
                y -= 1;
                break;
            }
            case 'd': {
                if (y >= cols - 2) return;
                y += 1;
                break;
            }
        }
    } else {
        switch (op) {
            case 'w': {
                if (px <= 1) return;
                px -= 1;
                break;
            }
            case 's': {
                if (px >= lines - 2) return;
                px += 1;
                break;
            }
            case 'a': {
                if (py <= 1) return;
                py -= 1;
                break;
            }
            case 'd': {
                if (py >= cols - 2) return;
                py += 1;
                break;
            }
        }
    }
}

void readCallback(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer) {
    int n = buffer->FindEnd();
    if (n != -1) {
        std::string msg = buffer->retriveSome(n - buffer->readIndex() + 1);
        std::cout << msg << std::endl;
        auto all = conn->loop()->TcpS();
        char tag = msg[0];
        if (tag != 'A' && tag != 'B') return;
        char op = msg[1];
        handlerMove(tag, op);
        if (tag == 'A') {
            for (auto& it : all) {
                it.second->send(tag + std::to_string(x) + ':' + std::to_string(y) + "\r\n\r\n");
            }
        } else {
            for (auto& it : all) {
                it.second->send(tag + std::to_string(px) + ':' + std::to_string(py) + "\r\n\r\n");
            }
        }
    }
}

std::vector<std::string> tags{"TAGA", "TAGB"};

void onEstab(const std::shared_ptr<TcpConnection>& conn) {
    static int i = 0;
    std::cout << "new connection" << std::endl;
    conn->send(tags[i] + "\r\n\r\n");
    i = (i + 1) % tags.size();
}


int main() {
    InetAddr addr("127.0.0.1", 9999);
    EventLoop loop;
    TcpServer server(&loop, addr);
    server.closeNagle();
    server.setOnMessageCallback(readCallback);
    server.setOnNewConnectionCallback(onEstab);
    server.setThreadNums(1);
    server.start();
    loop.loop();
}