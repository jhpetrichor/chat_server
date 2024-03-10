#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(9090);
    server_info.sin_addr.s_addr = inet_addr("10.32.64.247");
    int size = sizeof(server_info);

    if(connect(sockfd, (struct sockaddr *)&server_info, size) == -1) {
        perror("bind");
        exit(1);
    }

    const char *buf = "helloworld";
    send(sockfd, buf, 10, 0);
    while(1) {
    }

    return 0;
}
