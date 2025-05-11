#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serv;
    char *msg = "Hello from TCP client\n";
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);
    connect(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    write(sockfd, msg, strlen(msg));
    int n = read(sockfd, buf, sizeof(buf)-1);
    buf[n] = 0;
    printf("Echo: %s", buf);
    close(sockfd);
    return 0;
}
