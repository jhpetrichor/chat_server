#ifndef CHAT_DATABASE_H
#define CHAT_DATABASE_H

#include <iostream>
#include <mutex>
#include <mysql/mysql.h>
#include <vector>

class DataBase {
private:
    MYSQL *mysql;
    std::mutex _mutex;

public:
    DataBase();
    ~DataBase();

    bool database_connect();
    void database_disconnect();
    int database_get_group_info(std::vector<std::string>&);
};

#endif // !CHAT_DATABASE_H
