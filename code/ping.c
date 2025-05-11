#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>

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
    if (argc != 2) { printf("usage: %s <ip>\n", argv[0]); return 1; }
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct sockaddr_in addr = { .sin_family = AF_INET };
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    struct icmp icmp_hdr = { .icmp_type = ICMP_ECHO, .icmp_code = 0, .icmp_id = getpid() & 0xFFFF, .icmp_seq = 1 };
    char packet[64];
    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
    ((struct icmp*)packet)->icmp_cksum = checksum(packet, sizeof(packet));
    sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&addr, sizeof(addr));
    printf("Sent ICMP Echo to %s\n", argv[1]);
    close(sock);
    return 0;
}
