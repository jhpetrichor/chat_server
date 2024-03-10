#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "chat_database.h"
#include "chat_list.h"
#include "chat_thread.h"

#include <arpa/inet.h>
#include <event.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>

#define IP "10.32.64.247"
#define PORT 9090

class ChatServer {
private:
    struct event_base *base;
    DataBase *db;
    ChatInfo *info;
    ChatThread *pool;
    int thread_num;   // 线程数量
    int cur_thread;   // 当前线程
public:
    ChatServer();
    ~ChatServer();

    void listen(const char *, int);
    static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int, void *);
    void server_update_group_info();
    void server_alloc_event(evutil_socket_t);
};

#endif // CHAT_SERVER_H
