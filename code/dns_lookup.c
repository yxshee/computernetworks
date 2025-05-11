#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { printf("Usage: %s <hostname>\n", argv[0]); return 1; }
    struct addrinfo hints = { .ai_family = AF_UNSPEC }, *res, *p;
    if (getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
        perror("getaddrinfo"); return 1;
    }
    for (p = res; p; p = p->ai_next) {
        char ip[INET6_ADDRSTRLEN];
        void *addr = (p->ai_family == AF_INET)
            ? (void*)&((struct sockaddr_in*)p->ai_addr)->sin_addr
            : (void*)&((struct sockaddr_in6*)p->ai_addr)->sin6_addr;
        inet_ntop(p->ai_family, addr, ip, sizeof(ip));
        printf("%s\n", ip);
    }
    freeaddrinfo(res);
    return 0;
}
