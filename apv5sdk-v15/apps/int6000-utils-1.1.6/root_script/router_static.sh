ifconfig eth0 192.168.2.1
#/usr/sbin/udhcpd -f /root/udhcpd.conf
route del -net 192.168.2.0 netmask 255.255.255.0 dev eth1.2
route del default gw 192.168.2.1
route add default gw 192.168.2.1 dev eth0
iptables -t nat -F
iptables -A POSTROUTING -o eth0 -t nat -j MASQUERADE
echo 1 > /proc/sys/net/ipv4/ip_forward

