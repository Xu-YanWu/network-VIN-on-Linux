#pragma once
#include <netinet/ip_icmp.h>
#include <net/if.h>
#include <net/ethernet.h>      // 不可去掉
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    /* Source/destination address. */
    const unsigned char dst_address[ETH_ALEN];
    const unsigned char src_address[ETH_ALEN];

    /* Ethernet type. */
    unsigned short type;
} eth_header_t;

#ifdef __cplusplus  
extern "C" {
#endif

struct ifreq init_macsocket_recv(const char *device, int *rawsock);
int mac_recv(char *packet, int *rawsock, struct ifreq ifr_re,
    const unsigned char* src_mac, const unsigned char* dst_mac, unsigned int ethertype);

#ifdef __cplusplus  
}
#endif
