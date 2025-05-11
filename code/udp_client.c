#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serv;
    char *msg = "Hello from UDP client\n";
    char buf[1024];
    socklen_t servlen = sizeof(serv);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&serv, servlen);
    int n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, NULL, NULL);
    buf[n] = 0;
    printf("Echo: %s", buf);
    close(sockfd);
    return 0;
}
