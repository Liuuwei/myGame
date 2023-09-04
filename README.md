安装  
```c++
    mkdir build
    cd build
    cmake ..
    make
    cd ../bin
    ./server
    ./client
```

### 8.28 完成基础游戏功能，实现联网  
### 8.28-23:04 增加游戏内成长功能，完成游戏胜负判断  
### 8.31-01:17 
#### 修改：
客户端、服务器
#### 修改前：
玩家位置移动在本地进行，完成移动之后将新坐标发送给服务器，由服务器同步给其他玩家。这种情况下似乎会导致坐标不同步，对方的实际坐标会和自己现实的坐标有偏差。
#### 第一次修改：
客户端保存自己的坐标，每次移动之前判断下一个坐标是否合法，如果合法就向服务器发送新坐标，等得到服务器的确认之后再实际更新自己的坐标。这种做法在高频率的移动下会有问题，如果客户端在接受到服务器的应答之前多次发出移动请求，就会导致服务器接受到的新坐标一直是同一个，那么客户端认为自己移动了很多次，但是服务器得到的一直是同一个坐标。
#### 第二次修改：
服务器保存客户端的坐标，客户端每次移动的时候不发送实际坐标，发送要移动的方向。服务器接受到要移动的方向之后判断是否合法，如果合法就给客户端回复新坐标。因为客户端发送的是移动的方向，所以即使在得到服务器应答之前发送多次也不会有第一次修改的问题。  
### 9.4-12:50:  
重写游戏服务端和客户端