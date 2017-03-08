#include<stdio.h>
#include<memory.h>

// | dest mac | sour mac | frame type | hard type | protocol type | mac len | protocol len | sour mac | sour ip | dest mac | dest ip |
// |     6    |     6    |      2     |     2     |        2      |    1    |       1      |     6    |     4   |      6   |    4    |
// |          ethernet head           |                                 ARP request                                                  |

struct ethernet_head
{
  unsigned char dest_mac[6];
  unsigned char source_mac[6];
  unsigned short eh_type;
};

struct arp_head
{
  unsigned short hardware_type;
  unsigned short protocol_type;
  unsigned char add_len;
  unsigned char pro_len;
  unsigned short option;
  unsigned char sour_add[6];
  unsigned long sour_ip;
  unsigned char dest_addr[6];
  unsigned long dest_ip;
  unsigned char padding[18];
}

struct arp_packet
{
  ethernet_head eth;
  arp_head arp
}; 

unsigned char* BuildArpPacket(unsigned char *source_mac, unsigned long srcIP, unsigned long destIP)
{
  static struct arp_packet packet;
  memset
}

int main(int argc, char **argv)
{
  return 0
}
