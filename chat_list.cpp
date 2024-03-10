#include "chat_list.h"
#include <string.h>

ChatInfo::ChatInfo() {
    online_user = new std::list<User>;
    group_info = new std::map<std::string, std::list<std::string>>;
}

ChatInfo::~ChatInfo() {
    if (online_user) {
        delete online_user;
    }

    if(group_info) {
        delete group_info;
    }
}

void splitString(const std::string& input, char delimiter, std::vector<std::string>& splitStrings) {
    // std::vector<std::string> *result = new std::vector<std::string>;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        splitStrings.push_back(token);
    }
}


void ChatInfo::list_update_group(std::vector<std::string> &g, int num) {
    // std::cout << "group number: " << num << std::endl;    
    // if(group_info == NULL) {
    //     group_info = new std::map<std::string, std::list<std::string>>;
    // }
    for(const auto& s: g) {
        // 分割
        std::vector<std::string> sub_str;
        splitString(s, '|', sub_str);
        // groupname
        std::string groupname = sub_str[0];
        // member_list
        std::list<std::string> member_list(sub_str.begin() + 1, sub_str.end());
        group_info->insert({groupname, member_list});
    }

    list_print_group();
}

void ChatInfo::list_print_group() const {
    int i = 1;
    for(const auto& it: *group_info) {
        std::cout << i++ << "、" << it.first << ": ";
        for(const auto& member: it.second) {
            std::cout << member << " ";
        }
        std::cout << std::endl;
    }    
}
