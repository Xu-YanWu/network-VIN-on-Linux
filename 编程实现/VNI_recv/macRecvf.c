#include "macRecvf.h"


struct ifreq init_macsocket_recv(const char *device, int *rawsock){

    *rawsock = socket(AF_PACKET, SOCK_RAW, ntohs(ETH_P_ALL));
    if (*rawsock < 0)
    {
        printf("raw socket error!\n");
        exit(1);
    }

    struct ifreq ifr_re;
    strncpy(ifr_re.ifr_name, device, IFNAMSIZ);

    // 获取接口index
    if (-1 == ioctl(*rawsock, SIOCGIFINDEX, (char *)&ifr_re))
    {
        perror("get if index error");
        exit(1);
    }

    struct sockaddr_ll hwaddr;
    memset(&hwaddr, 0, sizeof(hwaddr));
    hwaddr.sll_ifindex = ifr_re.ifr_ifindex;
    hwaddr.sll_family = AF_PACKET;
    hwaddr.sll_protocol = htons(ETH_P_ALL);
    hwaddr.sll_hatype = 0;
    hwaddr.sll_pkttype = PACKET_HOST;
    hwaddr.sll_halen = ETH_ALEN;
    // printf("sll_ifindex : %d\n", hwaddr.sll_ifindex);
    // 绑定socket至接口
    if (-1 == bind(*rawsock, (struct sockaddr *)&hwaddr, sizeof(hwaddr)))
    {
        perror("raw socket bind error!\n");
        exit(1);
    }
    // 获取接口MAC地址
    if (-2 == ioctl(*rawsock, SIOCGIFHWADDR, (char *)&ifr_re))
    {
        perror("raw socket get mac address failed!\n");
        exit(1);
    }
    // printf("Local mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
    //        ifr_re.ifr_hwaddr.sa_data[0],
    //        ifr_re.ifr_hwaddr.sa_data[1],
    //        ifr_re.ifr_hwaddr.sa_data[2],
    //        ifr_re.ifr_hwaddr.sa_data[3],
    //        ifr_re.ifr_hwaddr.sa_data[4],
    //        ifr_re.ifr_hwaddr.sa_data[5]);

    return ifr_re;
}

int mac_recv(char *packet, int *rawsock, struct ifreq ifr_re,
    const unsigned char* src_mac, const unsigned char* dst_mac, unsigned int ethertype){
    
    static int BUFFSIZE = 1514;

    int recvresult = 0;
    unsigned char rbuf[BUFFSIZE];
    memset(rbuf, 0, sizeof(rbuf));
    recvresult = recvfrom(*rawsock, rbuf, BUFFSIZE, 0, NULL, NULL);
    if (recvresult < 0)
    {
        printf("receive error!\n");
        exit(1);
    }
        
    eth_header_t *eth;
    eth = (eth_header_t *)rbuf;

    if (eth->type == htons(ethertype))
    {
        // mac_compare(eth);
        if(strcmp((eth->src_address), src_mac)== 0 && \
        strcmp(eth->dst_address, dst_mac) == 0){
            // printf("content of buff %x , row_recv %d\n", rbuf[BUFFSIZE - 1], recvresult);
            memcpy(packet, rbuf + ETH_HLEN, recvresult - ETH_HLEN);
            recvresult -= ETH_HLEN;
        }else{
            recvresult = 0;
        }
    }
    else{
        recvresult = 0;
    }
    
    memset(rbuf, 0, sizeof(rbuf));

    return recvresult;
}
