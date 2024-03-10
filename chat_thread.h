#ifndef CHAT_THREAD_H
#define CHAT_THREAD_H

#include "chat_database.h"
#include "chat_list.h"

#include <thread>
#include <event.h>

class ChatThread {
private:
    // 线程对象
    std::thread *_thread;    
    std::thread::id _id;
    struct event_base *base;
    ChatInfo *info;
    DataBase *db;
public:
    ChatThread();
    ~ChatThread();
    void start(ChatInfo *, DataBase *);
    static void worker(ChatThread *);
    void run();
    static void timeout_cb(evutil_socket_t, short, void *);
    std::thread::id get_thread_id() const;
    struct event_base* thread_get_base();
    static void thread_readcb(struct bufferevent *, void *);
    static void thread_eventcb(struct bufferevent *, short, void*);
};

#endif
