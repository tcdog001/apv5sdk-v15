#!/bin/bash
# file: arp-clear.sh

# ====================================================================
# environment
# --------------------------------------------------------------------

. arp-token.sh

# ====================================================================
# clear arp cache;
# --------------------------------------------------------------------


ip neighbor delete dev ${IF1} to ${IP2} lladdr ${NIC2} 
ip neighbor delete dev ${IF2} to ${IP1} lladdr ${NIC1}
ip neighbor show

# ====================================================================
# clear route cache;
# --------------------------------------------------------------------

# ip route add dev ${IF1} to ${IP2} lladdr ${NIC2} table default
# ip route add dev ${IF2} to ${IP1} lladdr ${NIC1} table default
# ip route show

# ====================================================================
# restore route;
# --------------------------------------------------------------------

route add -net ${NET1} netmask 255.255.255.0 dev ${IF1}  
route add -net ${NET2} netmask 255.255.255.0 dev ${IF2}  

# ====================================================================
# enable arp;
# --------------------------------------------------------------------

ip link set dev ${IF1} arp on
ip link set dev ${IF2} arp on

# ====================================================================
# disable send-to-self feature;
# --------------------------------------------------------------------

echo 0 > /proc/sys/net/ipv4/conf/${IF1}/loop
echo 0 > /proc/sys/net/ipv4/conf/${IF2}/loop

# ====================================================================
# enable WAN connection;
# --------------------------------------------------------------------

ifconfig ${IF0} up
ifconfig lo up
