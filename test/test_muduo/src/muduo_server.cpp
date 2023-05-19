/*
muduo网络库给用户提供了两个主要的类
TcpServer : 用于编写服务器程序的
TcpClient : 用于编写客户端程序的

epoll + 线程池
好处：能够把网络I/O的代码和业务代码区分开
            业务代码的暴露：用户的连接和断开  用户的可读写事件
*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional> // 绑定器
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*
基于muduo网络库开发服务器程序
    1、组合TcpServer对象
    2、创建EventLoop事件循环对象的指针
    3、明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
    4、当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
    5、设置合适的服务器端线程数量，muduo库会自己分配I/O线程和worker线程
*/
class ChatServer {
public:
    ChatServer(EventLoop* loop, // 事件循环 可以理解成 Reactor
               const InetAddress& listenAddr, // IP + Port
               const string& nameArg) // 服务器的名称
        : _server(loop, listenAddr, nameArg), _loop(loop) {
        
        /*
            为什么用回调函数 
            因为 普通函数 需要同时知道这件事什么地方发生以及这件事发生后怎么做 就可以指名道姓调用这个函数 
            但是 有些函数 什么时候发生 和 发生后怎么做 没有在一起
            比如 现在知道 新用户连接 和 已连接用户断开 怎么做 但是不知道这件事情什么时候发生
            需要 网络对端 发送 相应的网络数据 上报上来 才知道 有用户的连接和断开
            谁来监听 这些事件 由网络库来监听 当网络库接受到网络对端发送过来的业务请求 进行上报
            现在只知道 事件发生后该怎么处理 不知道事件什么时候发生
            所以 注册回调 当相应的事件发生以后 就会帮我们去调用这个回调函数 只要关注在回调函数里开发业务
        */

        // 给服务器注册用户链接的创建和断开回调
        // 现在写的是一个成员方法onConnection 是为了访问对象的成员变量 非静态成员方法会有一个this指针 总共两个参数
        // setConnectionCallback 的形参是函数对象的引用cb  const ConnectionCallback& cb 
        // 函数对象类 ConnectionCallback 只有一个参数 typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback
        // 与给的成员方法onConnection类型不同 需要用绑定器绑定 在类内 用this指针传对象的地址 在类外就得用&obj
        // _1 占位符 代表形参 ConnectionCallback类的 函数对象cb 的第一个参数 const TcpConnectionPtr&
        // 这里的含义其实是
        // setConnectionCallback函数的形参为 类型为ConnectionCallback 的 函数对象的引用 cb   ->   const ConnectionCallback& cb
        // typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback 函数对象类 -> ConnectionCallback 的定义
        // std::function<void(const TcpConnectionPtr&)> ConnectionCallback& cb = std::bind(&ChatServer::onConnection, this, _1)
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1)); 

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

        // 设置服务器端的线程数量  1个I/O线程  3个worker线程
        _server.setThreadNum(4);
    }

    // 开启事件循环
    void start() {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开  epoll listenfd accept  相当于底层的muduo库将这些都封装了 只暴露了一个回调的接口 onConnection 不用管什么时候调用 注册到muduo库 当有相应的事件 这个方法就会响应
    void onConnection(const TcpConnectionPtr& conn) { // TcpConnectionPtr 是一个智能指针 里面包含了一个个TCP连接
        if(conn->connected()) {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:online" << endl;
        } else {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:offline" << endl;
            conn->shutdown(); // close(fd) 把socket()释放掉 服务端epoll的资源回收
            // _loop->quit(); // 不想再提供服务 相当于退出 epoll 整个服务器结束
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr& conn, // 连接
                   Buffer* buffer, // 缓冲区
                   Timestamp time) { // 接受到数据的时间信息
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << " time:" << time.toString() << endl;
        conn->send(buf);
    }
    // 把主要的精力放在 onConnection 和 onMessage 上 不用关注其他网络代码

    TcpServer _server; // #1
    EventLoop* _loop;  // #2  可以看成 epoll 是事件循环 如果有相应的事件发生了 loop会上报
};

int main() {
    EventLoop loop; // 很像 epoll_create 创建epoll 
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");
    
    server.start(); // 启动 把listenfd 用 epoll_ctl 添加到 epoll
    loop.loop(); // 调用 epoll_wait 以阻塞的方式等待新用户连接、已连接用户的读写事件等

    return 0;
}