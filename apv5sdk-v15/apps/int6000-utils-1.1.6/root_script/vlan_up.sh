#; 802.1q mode
ethreg -i eth1 0x108=0xc03e0001
#; port-base mode p0/1
ethreg -i eth1 0x208=0x00010002
#; port-base mode p0/2/3/4
ethreg -i eth1 0x308=0x00190001
#; port-base mode p0/2/3/4
ethreg -i eth1 0x408=0x00150001
#; port-base mode p0/2/3/4
ethreg -i eth1 0x508=0x000d0001
ethreg -i eth1 0x44=0x00000803
#; 802.1q vlan group p0/1 for VID=2
ethreg -i eth1 0x40=0x0002000a
ethreg -i eth1 0x44=0x0000081d
#; 802.1q vlan group p0/2/3/4 for VID=1
ethreg -i eth1 0x40=0x0001000a
ethreg -i eth1 0x104=0x00004204
ethreg -i eth1 0x204=0x00004104
ethreg -i eth1 0x304=0x00004104
ethreg -i eth1 0x404=0x00004104
ethreg -i eth1 0x504=0x00004104
ethreg -i eth1 0x30=0x21f005f2

vconfig add eth1 1
vconfig add eth1 2
ifconfig eth1.1 up
ifconfig eth1.2 up
brctl delif br0 eth1
brctl addif br0 eth1.1
