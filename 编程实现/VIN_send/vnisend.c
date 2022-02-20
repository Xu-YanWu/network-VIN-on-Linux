#include "macRecvf.h"
#include "macSendf.h"


#define Ethertype 0xf4f0
#define vniHead_len 4 // 学号 长度
#define vniOrder_len 2 // 序号 长度


const char* device = "ens33";    //网路设备名称，由cat /proc/net/dev 命令可以查看
const char dst_mac[6] = {0x00, 0x0c, 0x29, 0x08, 0xab, 0x10};
const char src_mac[6] = {0x00, 0x0c, 0x29, 0xb0, 0x64, 0xd3};
const char broad_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned int pkt_size = 1514;

const char *my_ip = "10.168.1.185";
const char *dst_ip = "10.168.1.210";

const char vni_head_a[vniHead_len] = {0x00, 0x03, 0x00, 0x06}; // 学号

int main(){
    // printf("ok1\n");
    char recv_packet[pkt_size];
    int recv_size;
    int recvsock;
    struct ifreq ifr_mre;
    ifr_mre = init_macsocket_recv(device, &recvsock);
    
    char send_packet[pkt_size];
    int sendsock;
    int sendlen = 0;
    unsigned int order = 0;
    struct ifreq ifr_mse;
    ifr_mse = init_macsocket_send(device, &sendsock, src_mac);
    memset(send_packet, 0, sizeof(send_packet));

    struct vnihdr *vni_header;
    vni_header = (struct vnihdr *)malloc(sizeof(struct vnihdr));

    // printf("ok2\n");
    while (1)
    {
        recv_size = mac_recv(recv_packet, &recvsock, ifr_mre, src_mac, dst_mac);

        struct ip *ip_str = (struct ip *)recv_packet;

        
        // if (strcmp(inet_ntoa(ip_str->ip_dst), dst_ip) == 0 && strcmp(inet_ntoa(ip_str->ip_src), my_ip) == 0 \
        //     && ip_str->ip_p == 1){               
        if (ip_str->ip_p == 1){                           
            // printf("ok3\n");
            memcpy(vni_header->numbers, vni_head_a, vniHead_len);
            // printf("ok4\n");
            vni_header->seq = (order)%65536;
            
            memcpy(send_packet, vni_header, vniHead_len + vniOrder_len);
            
            memcpy(send_packet + vniHead_len + vniOrder_len, recv_packet, recv_size);
            sendlen = mac_send(send_packet, vniHead_len + vniOrder_len + recv_size, \
                                &sendsock, ifr_mse, src_mac, broad_mac, Ethertype);
            printf("%d packet have been send\n", order);
            order++;
        }
        
        memset(recv_packet, 0, sizeof(recv_packet));
        memset(send_packet, 0, sizeof(send_packet));
    }
   
    
    
    return 0;
}
