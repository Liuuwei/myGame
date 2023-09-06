## 依赖
```
tinymuduo库  
ncurses库
```  
## 安装  
```
    mkdir build
    cd build
    cmake ..
    make
    cd ../bin
    ./server
    ./client
```
## 更新日志
```
8.28:
完成基础游戏功能，实现联网
  
9.4-12:50: 
重写游戏服务端和客户端

9.4-21:41:
优化游戏客户端，修复bug，回调函数只会在接受到新数据的时候触发，服务器可能会有多条指令同时到达，所以客户端需要在回调
函数中读完所以指令

9.6-15:36:
完成游戏功能，完成Player类，修改GameServer，修改GameClient
实现了多人在线绘画的功能，每个人可以实时看到别人的画
```  

## 展示  
![](/Users/liuwei/CLionProjects/game/show/gameshow.jpg)