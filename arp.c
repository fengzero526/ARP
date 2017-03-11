#include<stdio.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<errno.h>
#include<memory.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<net/ethernet.h>

#define ETHER_HEADER_LEN sizeof(struct ether_header)
#define ETHER_ARP_LEN sizeof(struct ether_arp)
#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN+ETHER_ARP_LEN
#define IP_ADDR_LEN 4
#define MAC_ADDR_LEN 6

struct ether_arp *fill_arp_packet(const unsigned char *src_mac_addr, const char *src_ip, const char *dst_ip){
  struct ether_arp *arp_packet;
  struct in_addr src_in_addr,dst_in_addr;
  inet_pton(AF_INET, src_ip, &src_in_addr);  
  inet_pton(AF_INET, dst_ip, &dst_in_addr);

  arp_packet = (struct ether_arp *)malloc(ETHER_ARP_LEN);
  arp_packet->arp_hrd = htons(ARPHRD_ETHER);
  arp_packet->arp_pro = htons(ETHERTYPE_IP);
  arp_packet->arp_hln = MAC_ADDR_LEN;
  arp_packet->arp_pln = IP_ADDR_LEN;
  arp_packet->arp_op = htons(ARPOP_REQUEST);
  
  memcpy(arp_packet->arp_sha, src_mac_addr,MAC_ADDR_LEN);
  memset(arp_packet->arp_tha,0xff,6);
  memcpy(arp_packet->arp_spa, &src_in_addr, IP_ADDR_LEN);
  memcpy(arp_packet->arp_tpa, &dst_in_addr, IP_ADDR_LEN);
  
  return arp_packet;
}

void arp_request(const char *if_name, const char *dst_ip){
  struct sockaddr_ll saddr_ll;
  struct ether_header *eth_header;
  struct ether_arp *arp_packet;
  struct ifreq ifr;

  char buf[ETHER_ARP_PACKET_LEN+18];
  unsigned char src_mac_addr[MAC_ADDR_LEN];
  char *src_ip;
  int sock_raw_fd, ret_len, i;

  sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if (sock_raw_fd == -1 ){
    printf("socket error: %s\n",strerror(errno));
  }
  
  bzero(&saddr_ll, sizeof(struct sockaddr_ll));
  bzero(&ifr, sizeof(struct ifreq));

  memcpy(ifr.ifr_name, if_name, strlen(if_name));  
  
  if(-1 == ioctl(sock_raw_fd,SIOCGIFINDEX, &ifr)){
    printf("get eth error: %s\n",strerror(errno));
  }
  
  saddr_ll.sll_ifindex = ifr.ifr_ifindex;
  saddr_ll.sll_family = PF_PACKET;

  ioctl(sock_raw_fd, SIOCGIFADDR, &ifr);  
  src_ip = inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);

  ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr);
  memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data,MAC_ADDR_LEN);

  bzero(buf, ETHER_ARP_PACKET_LEN);
  eth_header = (struct ether_header *)buf;
  memcpy(eth_header->ether_shost, src_mac_addr, MAC_ADDR_LEN);
  memset(eth_header->ether_dhost, 0xff, MAC_ADDR_LEN);
  eth_header->ether_type = htons(ETHERTYPE_ARP);

  arp_packet = fill_arp_packet(src_mac_addr, src_ip, dst_ip);
  memcpy(buf+ETHER_HEADER_LEN, arp_packet, ETHER_ARP_LEN);
  memset(buf+42,0,18);

  ret_len  = sendto(sock_raw_fd, buf, ETHER_ARP_PACKET_LEN+18, 0, (struct sockaddr *)&saddr_ll, sizeof(struct sockaddr_ll));
  if(ret_len>0)
    printf("send ok: %d\n",ret_len);
  
  close(sock_raw_fd);
}

int main(int argc, char** argv){
   arp_request(argv[1],argv[2]); 
}
