#include "chat_database.h"

DataBase::DataBase() { mysql = new MYSQL; }

DataBase::~DataBase() {}

bool DataBase::database_connect() {
    // 初始化数据库句柄
    mysql = mysql_init(NULL);
    // 连接数据库
    mysql_real_connect(mysql, "172.17.0.2", "root", "root", "chat_database", 0,NULL, 0);
    if (mysql == NULL) {
        std::cout << "[error] mysql init failed!" << std::endl;
        return false;
    }

    // 设置编码格式
    if (mysql_query(mysql, "set names utf8;") != 0) {
		std::cout << mysql_query(mysql, "set names utf8;") << std::endl;
        std::cout << "[error] set names utf8 error" << std::endl;
    }
    return true;
}

void DataBase::database_disconnect() { 
    mysql_close(mysql);
}


int DataBase::database_get_group_info(std::vector<std::string>& groupinfo) {
    if(mysql_query(mysql, "SELECT * FROM chat_group;") != 0) {
        std::cout << "[error] select error" << std::endl;
        return -1;
    }

    // 获取结果
    MYSQL_RES *res = mysql_store_result(mysql);
    if(NULL == res) {
        std::cout << "[error] store result error" << std::endl;
        return -1;
    }

    MYSQL_ROW r;
    while((r = mysql_fetch_row(res))) {
        std::string s;
        s += r[0];
        s += '|';
        s += r[2];
        // std::cout << idx+1 << groupinfo[idx] << std::endl;
        groupinfo.push_back(s);
    }

    mysql_free_result(res);
    return groupinfo.size();
}
