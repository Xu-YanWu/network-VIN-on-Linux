#include "macRecvf.h"


#define Ethertype 0xf4f0

const char* device = "ens33";    //网路设备名称，由cat /proc/net/dev 命令可以查看
// const unsigned char src_mac[6] = {0x00, 0x0c, 0x29, 0x08, 0xab, 0x10};
const unsigned char src_mac[6] = {0x00, 0x0c, 0x29, 0xdb, 0x39, 0x41};
const unsigned char dst_mac[6] = {0x00, 0x0c, 0x29, 0xdb, 0x39, 0x41};
const unsigned char broad_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned int pkt_size = 1514;

// const char *dst_ip = "10.168.1.207";
const char *dst_ip = "10.168.1.210";
// const char *src_ip = "10.168.1.210";
const char *src_ip = "10.168.1.207";

// const char vin_head_a[4] = {0x00, 0x03, 0x00, 0x06};
const char vin_head_a[4] = {0x00, 0x03, 0x03, 0x00};
const unsigned int vinHead_len = 4;
const unsigned int vinOrder_len = 2;

int main(){
 
    char recv_packet[pkt_size];
    int recv_size;
    int recvsock;
    struct ifreq ifr_mre;
    ifr_mre = init_macsocket_recv(device, &recvsock);
    
    char rvin_head_a[4];
    char rvin_head_b[2];
    unsigned int order = 0;

    while (1)
    {
        recv_size = mac_recv(recv_packet, &recvsock, ifr_mre, src_mac, broad_mac, Ethertype);
        memcpy(rvin_head_a, recv_packet, vinHead_len);
        if(strcmp(rvin_head_a, vin_head_a) != 0){
            memset(recv_packet, 0, sizeof(recv_packet));
            continue;
        }
        memcpy(rvin_head_b, recv_packet + vinHead_len, vinOrder_len);
        struct ip *ip_str = (struct ip *)(recv_packet + vinHead_len + vinOrder_len);
        if (strcmp(inet_ntoa(ip_str->ip_dst), dst_ip) == 0 && strcmp(inet_ntoa(ip_str->ip_src), src_ip) == 0 \
            && ip_str->ip_p == 1)
        {                
            order = (int)(rvin_head_b[0]) * 256 + (int)(rvin_head_b[1]);
            printf("%d packet have been received\n", order);
        }
        
        memset(recv_packet, 0, sizeof(recv_packet));
    }
    
    
    return 0;
}
