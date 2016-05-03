ethreg -i eth1 0x00000108=0x003e0001
ethreg -i eth1 0x00000208=0x003d0001
ethreg -i eth1 0x00000308=0x003b0001
ethreg -i eth1 0x00000408=0x00370001
ethreg -i eth1 0x00000508=0x002f0001
#ethreg -i eth1 0x00000044=0x00000000
#ethreg -i eth1 0x00000040=0x00000000
#ethreg -i eth1 0x00000044=0x00000803
#ethreg -i eth1 0x00000040=0x00020002
ethreg -i eth1 0x00000104=0x00004004
ethreg -i eth1 0x00000204=0x00004004
ethreg -i eth1 0x00000304=0x00004004
ethreg -i eth1 0x00000404=0x00004004
ethreg -i eth1 0x00000504=0x00004004
ethreg -i eth1 0x40=0x00000009
brctl delif br0 eth1.1
ifconfig eth1.1 down
ifconfig eth1.2 down
vconfig rem eth1.1
vconfig rem eth1.2
brctl addif br0 eth1

