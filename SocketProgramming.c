#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define PACKET_SIZE 64

// ICMP header structure
struct icmpheader {
    unsigned char type;
    unsigned char code;
    unsigned short checksum;
    unsigned short id;
    unsigned short sequence;
};

// Checksum calculation function
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main() {
    int sock;
    struct sockaddr_in dest;
    char packet[PACKET_SIZE];
    struct iphdr *iph = (struct iphdr *) packet;
    struct icmphdr *icmph = (struct icmphdr *) (packet + sizeof(struct iphdr));

    // Create raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Zero out the packet buffer
    memset(packet, 0, PACKET_SIZE);

    // Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(PACKET_SIZE);
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_ICMP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.1.2"); // Replace with your source IP
    iph->daddr = inet_addr("192.168.1.1"); // Replace with your destination IP

    // IP checksum
    iph->check = checksum((unsigned short *)packet, iph->tot_len);

    // Fill in the ICMP Header
    icmph->type = ICMP_ECHO;
    icmph->code = 0;
    icmph->checksum = 0;
    icmph->un.echo.id = htons(12345);
    icmph->un.echo.sequence = htons(1);

    // ICMP checksum
    icmph->checksum = checksum((unsigned short *)icmph, sizeof(struct icmphdr));

    // Destination address
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("192.168.1.1"); // Replace with your destination IP

    // Send the packet
    if (sendto(sock, packet, PACKET_SIZE, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
    } else {
        printf("Packet Sent\n");
    }

    close(sock);
    return 0;
}
