#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in serv, cli;
    socklen_t clilen = sizeof(cli);
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    listen(sockfd, 5);
    printf("TCP server listening on port 8080...\n");
    newsock = accept(sockfd, (struct sockaddr*)&cli, &clilen);
    while (1) {
        int n = read(newsock, buf, sizeof(buf)-1);
        if (n <= 0) break;
        buf[n] = 0;
        printf("Received: %s", buf);
        write(newsock, buf, n);
    }
    close(newsock);
    close(sockfd);
    return 0;
}
