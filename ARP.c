#include<stdio.h>
#include<memory.h>
#include"ARP.h"

unsigned char* BuildArpPacket(unsigned char *source_mac, unsigned long srcIP, unsigned long destIP)
{
  static struct arp_packet packet;
  memset(packet.eth.dest_mac, 0xFF, 6);
  return (unsigned char*)&packet;
}

int main(int argc, char **argv)
{
  return 0;
}
