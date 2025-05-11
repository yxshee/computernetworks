#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serv, cli;
    socklen_t clilen = sizeof(cli);
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8081);
    bind(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    printf("UDP server listening on port 8081...\n");
    while (1) {
        int n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cli, &clilen);
        buf[n] = 0;
        printf("Received: %s", buf);
        sendto(sockfd, buf, n, 0, (struct sockaddr*)&cli, clilen);
    }
    close(sockfd);
    return 0;
}
