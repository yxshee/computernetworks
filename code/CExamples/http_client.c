#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main(int argc, char *argv[]) {
    if (argc != 2) { printf("Usage: %s <IPv4>\n", argv[0]); return 1; }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_port   = htons(PORT);
    inet_pton(AF_INET, argv[1], &serv.sin_addr);
    connect(sock, (struct sockaddr*)&serv, sizeof(serv));
    char *req = "GET / HTTP/1.1\r\nHost: \r\n\r\n";
    write(sock, req, strlen(req));
    char buf[4096];
    int n;
    while ((n = read(sock, buf, sizeof(buf)-1)) > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }
    close(sock);
    return 0;
}
