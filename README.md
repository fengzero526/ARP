#ARP
##ARP format
|dest mac|sour mac|frame type|hard type|protocol type|mac len|protocol len|op|sour mac|sour ip|dest mac|dest ip|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|6|6|2|2|2|1|1|2|6|4|6|4|

***
###ARP parameter
dest mac: 0xFF

sour mac: nil

frame type: 0x0806

hard type: 0x1(mac)

protocol type: 0x0800(ip)

mac len: 0x6

protocol len: 0x4

op: 1(arp request) 2(arp ack) 3(rarp request) 4(rarp ack)

 





