#include "chat_thread.h"
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <thread>

ChatThread::ChatThread() {
    _thread = new std::thread(worker, this);
    _id = _thread->get_id();
    base = event_base_new();
}


ChatThread::~ChatThread() {
    if(_thread) {
        delete _thread;
    }
}

void ChatThread::worker(ChatThread *t) {
    t->run();    
}

void ChatThread::run() {
    // 集合中放入一个定时器事件
    struct event timeout;
    struct timeval tv;
    
    event_assign(&timeout, base, -1, EV_PERSIST, timeout_cb, this);
    evutil_timerclear(&tv);
    tv.tv_sec = 3;
    // event_add(&timeout, &tv);

    std::cout << "--- thread " << _id << " start working ..." << std::endl;
        
    event_base_dispatch(base);
    event_base_free(base);
}

void ChatThread::timeout_cb(evutil_socket_t fd, short event, void *arg) {
    ChatThread *t = (ChatThread *)arg;
    // std::cout << "--- thread " << t->get_thread_id() << " is listening ..." << std::endl;    
}

void ChatThread::start(ChatInfo *_info, DataBase *_db) {
    this->info = _info;
    this->db = _db;    
}

std::thread::id ChatThread::get_thread_id() const {
    return _id;
}

struct event_base* ChatThread::thread_get_base() {
    return base;
}

void ChatThread::thread_readcb(struct bufferevent *bev, void *arg) {
    // char buf[1024] = {0};
    // bufferevent_read(bev, buf, 1024);    
    // std::cout << buf << std::endl;
}

void ChatThread::thread_eventcb(struct bufferevent *bev, short what, void *arg){
    
}
