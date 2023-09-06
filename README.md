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
```
## 使用
```
test目录下有服务端和客户端的使用代码，需要修改一下ip和端口号
cmake之后可执行文件在bin目录下
./server
./client
操作说明:
WASD移动，I绘画模式，C橡皮擦模式，L观察模式
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
![](https://github.com/Liuuwei/myGame/blob/main/show/gameshow.jpg?raw=true)
