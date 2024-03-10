cmake_minimum_required(VERSION 3.12)
project(ChatServer)

set(CMAKE_CXX_STANDARD 14)

# 添加源文件
set(SOURCES
        main.cpp
        chat_server.cpp
        chat_list.cpp
        chat_database.cpp
)

# 生成可执行文件
add_executable(main ${SOURCES})

# 链接所需的库
target_link_libraries(main PRIVATE event mysqlclient)