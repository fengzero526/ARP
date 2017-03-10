#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<memory.h>
#include<sys/socket.h>
#include"arp.h"

int main(int argc, char** argv)
{
  if(argc != 3){
    printf("Usage: fake_ip  fak_mac\n");
    return -1;
  }
  
  char* fakeIp;
  char* mac;

 // fakeIp = inet_addr(argv[1]);
  fakeIp = argv[1];
  mac = argv[2];

  int s;
  s = socket(PF_PACKET, SOCK_RAW, htons(0x0003));
  if(s<0){
    printf("socket raw error: %s\n",strerror(errno));
    return -1;
  }
  printf("socket ok\n");
  printf("send arp packet: ip: %s, mac: %s", fakeIp, mac);
  
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
