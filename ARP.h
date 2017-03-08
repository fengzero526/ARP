// create by fengzero 2017.3.8

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
};

struct arp_packet
{
  struct ethernet_head eth;
  struct arp_head arp;
};

unsigned char* BuildArpPacket(unsigned char *source_mac, unsigned long srcIP, unsigned long destIP);
