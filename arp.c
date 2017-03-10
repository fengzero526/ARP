#include<stdio.h>
#include<netinet/if_ether.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<memory.h>
#include<netpacket/packet.h>
#include<sys/socket.h>
#include"arp.h"

int main(int argc, char** argv)
{
  if(argc != 2){
    printf("Usage: fake_ip  fak_mac\n");
    return -1;
  }
  
  char* fakeIp;
  char* desIp;
  char* mac;

  fakeIp = inet_addr("192.168.0.1");
  desIp = inet_addr(argv[1]);
  mac = "7c:04:d0:c5:9b:48"

  int s;
  s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if(s<0){
    printf("socket raw error: %s\n",strerror(errno));
    return -1;
  }

  struct sockaddr_ll saddr_ll;
  saddr_ll.sll_ifindex = IF_PORT_100BASET;
  saddr_ll.sll_family = PF_PACKET;

  int ret;
  char* pack;
  pack = BuildArpPacket(mac, fakeIp, desIp); 
  ret = sendto(s, pack, sizeof(*pack), 0, (struct sockaddr*)&saddr_ll, sizeof(struct sockaddr_ll));
  if(ret > 0){
    printf("send ok\n");
  }
  close(s);
  return 0;
}

unsigned char* BuildArpPacket(unsigned char *source_mac, unsigned long srcIP, unsigned long destIP)
{
  static struct arp_packet packet;
  memset(packet.eth.dest_mac, 0xFF, 6);
  memcpy(packet.eth.source_mac, source_mac, 6);
  packet.eth.eh_type = htons(0x0806);
  
  packet.arp.hardware_type = htons(0x0001);
  packet.arp.protocol_type = htons(0x0800);
  packet.arp.add_len = 0x06;
  packet.arp.add_len = 0x04;
  packet.arp.option = htons(0x0001);
  memcpy(packet.arp.sour_addr, source_mac, 6);
  packet.arp.sour_ip = srcIP;
  memset(packet.arp.dest_addr, 0, 6);
  packet.arp.dest_ip = destIP;
  memset(packet.arp.padding, 0, 18);

  return (unsigned char*)&packet;
}
