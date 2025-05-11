#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#define MAXHOPS 30

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    for (; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { printf("Usage: %s <destination IP>\n", argv[0]); return 1; }
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct sockaddr_in dest = { .sin_family = AF_INET };
    inet_pton(AF_INET, argv[1], &dest.sin_addr);

    for (int ttl = 1; ttl <= MAXHOPS; ttl++) {
        setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        struct icmp hdr = {
            .icmp_type = ICMP_ECHO,
            .icmp_code = 0,
            .icmp_id   = getpid() & 0xFFFF,
            .icmp_seq  = ttl
        };
        char pkt[64];
        memcpy(pkt, &hdr, sizeof(hdr));
        ((struct icmp*)pkt)->icmp_cksum = checksum(pkt, sizeof(pkt));

        struct timeval start, end;
        gettimeofday(&start, NULL);
        sendto(sock, pkt, sizeof(pkt), 0, (struct sockaddr*)&dest, sizeof(dest));

        socklen_t len = sizeof(dest);
        char buf[512];
        int n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&dest, &len);
        gettimeofday(&end, NULL);

        if (n > 0) {
            double ms = (end.tv_sec - start.tv_sec)*1000.0 +
                        (end.tv_usec - start.tv_usec)/1000.0;
            char hopip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &dest.sin_addr, hopip, sizeof(hopip));
            printf("%2d  %s  %.3f ms\n", ttl, hopip, ms);
            if (strcmp(hopip, argv[1]) == 0) break;
        }
    }
    close(sock);
    return 0;
}
