#include "chat_server.h"
#include "chat_thread.h"
#include "chat_database.h"
#include "chat_list.h"
#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_struct.h>
#include <event2/listener.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

ChatServer::ChatServer() {
    // 初始化时间集合
    base = event_base_new();
    // 初始化数据库对象
    db = new DataBase();
    info = new ChatInfo();
    // 初始化群信息， 把群信息从数据库中读取出来，放在map里面
    server_update_group_info();

    thread_num = 3;
    cur_thread = 0;
    pool = new ChatThread[thread_num];
    for(int i = 0; i < thread_num; ++i) {
        pool[i].start(info, db);
    } 
}

ChatServer::~ChatServer() {
    if(db) {
        delete db;
    }
    if(info) {
        delete info;
    }
}

void ChatServer::listen(const char *ip, int port) {
    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_port = htons(port);

    struct evconnlistener *listener = evconnlistener_new_bind(
          base, listener_cb, NULL, LEV_OPT_CLOSE_ON_FREE, 5,
        (struct sockaddr *)&server_info, sizeof(server_info));

    if (NULL == listener) {
        std::cout << "evconnlistener_new_bind error" << std::endl;
        exit(1);
    }

    // 监听集合
    event_base_dispatch(base);

    // 释放对象
    evconnlistener_free(listener);
    event_base_free(base);
}

// 回调函数： 有客户端发起连接会触发该函数
void ChatServer::listener_cb(struct evconnlistener *listener,
                             evutil_socket_t fd, struct sockaddr *c,
                             int socklen, void *arg) {
    ChatServer *ser = (ChatServer *)arg;
    struct sockaddr_in *client_info = (struct sockaddr_in *)c;
    std::cout << "[connection]";
    std::cout << "client ip: " << inet_ntoa(client_info->sin_addr) << " ";
    std::cout << "port: " << client_info->sin_port << std::endl;
    ser->server_alloc_event(fd);
}

void ChatServer::server_update_group_info() {
    if(!db->database_connect()){
        std::cout << "[error] database_connect error" << std::endl;
        exit(1);
    }

    std::vector<std::string> groupinfo;
    int num = db->database_get_group_info(groupinfo);
    // std::cout << "group number: " << num << std::endl;
    
    db->database_disconnect();

    info->list_update_group(groupinfo, num);
}


// #define int evutil_socket_t int
void ChatServer::server_alloc_event(evutil_socket_t fd) {
    struct event_base *t_base = pool[cur_thread].thread_get_base();
    cur_thread = (cur_thread + 1) % thread_num;
    struct bufferevent *bev = bufferevent_socket_new(t_base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, ChatThread::thread_readcb, NULL, ChatThread::thread_eventcb, NULL);
    bufferevent_enable(bev, EV_READ);
}

