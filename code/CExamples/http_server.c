#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000
const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 56\r\n"
    "\r\n"
    "<html><body><h1>Hello from C HTTP Server</h1></body></html>";

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    bind(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    listen(sockfd, 5);
    printf("HTTP server listening on port %d\n", PORT);
    while (1) {
        int client = accept(sockfd, NULL, NULL);
        char buf[1024];
        read(client, buf, sizeof(buf)-1);
        write(client, response, strlen(response));
        close(client);
    }
    close(sockfd);
    return 0;
}
