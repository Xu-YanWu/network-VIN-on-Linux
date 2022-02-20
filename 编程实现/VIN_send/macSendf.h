#pragma once
#include <net/ethernet.h>      // 不可去掉
// #include <linux/if_arp.h>
#include <linux/if_packet.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    struct ethhdr eth_header; //struct defined in linux/if_ether.h
} PACKET;

struct vnihdr
{
    char numbers[4];// 学号 4byte
    unsigned short seq;      // 序号 2byte
};

#ifdef __cplusplus  
extern "C" {
#endif

struct ifreq init_macsocket_send(const char *device, int *sockfd, const unsigned char* maymac);

int mac_send(char *mypkt, unsigned int len, int *sockfd, struct ifreq ifr,
    const unsigned char* maymac, const unsigned char* tarmac, unsigned int ethertype);

#ifdef __cplusplus  
}
#endif
