#include "macSendf.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>



struct ifreq init_macsocket_send(const char *device, int *sockfd, const unsigned char* maymac){
    
    *sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
    if(*sockfd < 0){
	    perror("socket error!");
        exit(1);
	}
    
    struct ifreq ifr;
    size_t if_name_len = strlen(device);
    if(if_name_len < sizeof(ifr.ifr_name)){
        // strncpy(ifr.ifr_name, device, IFNAMSIZ);
        memcpy(ifr.ifr_name,device,IFNAMSIZ);
    }else{
        perror("interface name is too long");
        exit(1);
    }
    if(-1 == ioctl(*sockfd,SIOCGIFINDEX,&ifr)) {
        perror("get if index error");
        exit(1);
    }

    struct sockaddr_ll my_hwaddr;
    my_hwaddr.sll_family = AF_PACKET;
    my_hwaddr.sll_protocol = htons(ETH_P_IP);
    my_hwaddr.sll_ifindex = ifr.ifr_ifindex; /*接口号2表示是eth0*/
    my_hwaddr.sll_pkttype = PACKET_HOST;
    my_hwaddr.sll_halen = ETH_ALEN;
    memcpy(my_hwaddr.sll_addr, maymac, ETH_ALEN);

    bind(*sockfd,(struct sockaddr *)&my_hwaddr,sizeof(my_hwaddr));

    return ifr;
}

int mac_send(char *mypkt, unsigned int len, int *sockfd, struct ifreq ifr,
    const unsigned char* maymac, const unsigned char* tarmac, unsigned int ethertype){
    
    unsigned char buff[ETH_HLEN + len];
    memset(buff, 0, sizeof(buff));
    PACKET* pkt;
    pkt = (PACKET * )buff;
    struct sockaddr_ll target_hwaddr;

    memcpy(pkt->eth_header.h_dest, tarmac, ETH_ALEN);
    memcpy(pkt->eth_header.h_source, maymac, ETH_ALEN);
    pkt->eth_header.h_proto = htons(ethertype);

    memcpy(buff+ETH_HLEN, mypkt, len);

    target_hwaddr.sll_family = AF_PACKET;
    target_hwaddr.sll_protocol = htons(ETH_P_IP);
    target_hwaddr.sll_ifindex = ifr.ifr_ifindex;
    target_hwaddr.sll_pkttype = PACKET_BROADCAST;
    target_hwaddr.sll_halen = ETH_ALEN;
    memcpy(target_hwaddr.sll_addr, tarmac, ETH_ALEN);

    int sendresult = 0;
    sendresult = sendto(*sockfd, buff, ETH_HLEN + len, 0, 
    (struct sockaddr *)&target_hwaddr,sizeof(target_hwaddr));
    if (sendresult == -1){
        perror("sendto error");
        exit(1);
    }
    
    memset(buff, 0, sizeof(buff));
    return sendresult;
}