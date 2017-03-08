#include<stdio.h>
#include<arpa/inet.h>
#include<memory.h>
#include"arp.h"

unsigned char* BuildArpPacket(unsigned char *source_mac, unsigned long srcIP, unsigned long destIP)
{
  static struct arp_packet packet;
  memset(packet.eth.dest_mac, 0xFF, 6);
  memcpy(packet.eth.source_mac, source_mac, 6);
  packet.eth.eh_type = htons(0x0806);
  return (unsigned char*)&packet;
}

int main(int argc, char **argv)
{
  return 0;
}
